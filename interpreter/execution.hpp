#pragma once

#include "type_system.hpp"

namespace ksi::interpreter
{

  struct vm_config;

}
namespace ksi::interpreter::execution
{

  struct sequence;
  using seq_pointer = sequence const *;

  struct thread;

  struct params
  {
    vm_config * vm_configuration;
    thread * h_tread;
  };

  union data_type
  {
    bool boolean;
    sys::integer integer;

    template <typename T>
    T get() const
    {
      if constexpr( std::is_same_v<T, bool> )
      {
        return this->boolean;
      }
      else if constexpr( std::is_same_v<T, sys::integer> )
      {
        return this->integer;
      }
      else
      {
        static_assert("This type is not supported");
      }

      return {};
    }

    template <typename T>
    T & get()
    {
      if constexpr( std::is_same_v<T, bool> )
      {
        return this->boolean;
      }
      else if constexpr( std::is_same_v<T, sys::integer> )
      {
        return this->integer;
      }
      else
      {
        static T value;
        static_assert("This type is not supported");
        return value;
      }
    }
  };

  struct instruction_with_function
  {
    static void empty_function(params & p, data_type const & data) {}

    using function = decltype( &empty_function );

    sys::sview  name;
    function    fn{ &empty_function };

    instruction_with_function & base() { return *this; }
  };

  struct instruction : instruction_with_function
  {
    data_type data;

    void call(params & p) const { fn(p, data); }
  };

  using instr_list = std::vector<execution::instruction>;

  struct group_instructions
  {
    instr_list  list_instructions;
  };

  using groups_type = std::list<group_instructions>;

}
namespace ksi::interpreter::configuration
{

  struct module_config : meta_information
  {
    struct variable_information
    {
      sys::literal                          name;
      type_system::hints::i_hint            * type_restriction;
      std::size_t                           id;
      sys::unique<execution::sequence>      seq_init;

      static inline const auto auto_increment{ &variable_information::id };

      using Less = detail::literal_less;
    };

    struct read_only_cell
    {
      sys::literal                      name;
      sys::unique<execution::sequence>  seq_init;
      type_system::brick::cell_type     value;
      std::size_t                       id;

      static inline const auto auto_increment{ &read_only_cell::id };

      using Less = detail::literal_less;
    };

    static_table<variable_information> variables;
    static_table<read_only_cell>       constants;

    module_config(meta_information info) : meta_information{ info } {}
  };

}
namespace ksi::interpreter::execution
{

  struct sequence : type_system::base::counted
  {
    std::vector<sys::literal> var_names;
    execution::groups_type    groups;

    virtual type_result get_type(type_system::info::static_data & sd) const override { return sd.t_sequence; }

    static sequence * from_value(type_system::base::i_value * value, type_system::info::static_data & sd)
    {
      return ((value->get_type(sd) == sd.t_sequence) ? static_cast<sequence *>(value) : nullptr);
    }
  };

}
namespace ksi::interpreter::execution::data
{

  struct stack
  {
    using stack_frame = type_system::clearance::unique<type_system::impl::impl_array>;

    std::list<stack_frame> frames;

    void add_frame(std::size_t size)
    {
      frames.emplace_back(
        std::make_unique<type_system::impl::impl_array>(size)
      );
    }
  };

  struct call_stack
  {
    struct position
    {
      seq_pointer seq;

      using group_pointer = typename groups_type::const_iterator;
      using instr_index = typename instr_list::size_type;

      group_pointer current_group;
      instr_index   instruction_index{};

      const instruction & get() { return current_group->list_instructions[instruction_index]; }
    };

    std::list<position> pos;

    void next_instruction()
    {
      if( (++pos.front().instruction_index) == pos.front().current_group->list_instructions.size() )
      {
        pos.pop_front();
      }
    }

    void execute_instruction(params & r)
    {
      pos.front().get().call(r);
    }

    void run(seq_pointer seq, params & p)
    {
      if( seq->groups.empty() ) { return; }
      pos.emplace_front(seq, seq->groups.begin() );

      while( ! pos.empty() )
      {
        execute_instruction(p);
        next_instruction();
      }
    }
  };

}
namespace ksi::interpreter::execution
{

  struct module_data
  {
    enum class state { not_ready, calculating, ready };

    configuration::module_config                                  * config;
    type_system::clearance::unique<type_system::impl::impl_array> variables = std::make_unique<type_system::impl::impl_array>();
    std::vector<state>                                            variables_statuses;

    module_data(configuration::module_config * h_config) : config{ h_config }
    {
      add_more_variables();
    }

    void add_more_variables()
    {
      std::size_t new_size = config->variables.pos.size();

      variables->elements.reserve(new_size);
      for( std::size_t i = variables->elements.size(); i < new_size; ++i )
      { variables->elements.emplace_back(&variables->point); }

      variables_statuses.resize(new_size, state::not_ready);
    }

    type_system::base::i_value * get_value(std::size_t index, execution::params & params)
    {
      switch( variables_statuses[index] )
      {
        case state::calculating:
        throw state::calculating;

        case state::not_ready: {
          execution::data::call_stack call_stack;
          call_stack.run(config->variables.pos[index]->seq_init.get(), params);
        }
        [[fallthrough]];

        case state::ready:
        break;
      }

      return variables->elements[index].get_value();
    }

    type_system::base::i_value * find_value(sys::literal name, params & p)
    {
      // find constant
      configuration::module_config::read_only_cell * constant = config->constants.find(name);
      if( constant != nullptr ) { return constant->value.keep.handle; }

      // find config variable
      configuration::module_config::variable_information * var_info = config->variables.find(name);
      if( var_info == nullptr ) { return nullptr; }

      if( variables->elements.size() < var_info->id )
      {
        add_more_variables();
      }

      return get_value(var_info->id, p);
    }
  };

}
namespace ksi::interpreter
{

  struct vm_config_settings
  {
    std::shared_ptr<sys::dictionary>                              dict = std::make_shared<sys::dictionary>();
    static_table<configuration::module_config, meta_information>  modules;

    configuration::module_config * module_main;
    configuration::module_config * module_global;

    sys::literal literal_module_main{ dict->add(sys::string{"@main"sv}).pointer };
    sys::literal literal_module_global{ dict->add(sys::string{"@global"sv}).pointer };
    sys::literal literal_do{ dict->add(sys::string{"do"sv}).pointer };

    vm_config_settings()
    {
      module_main = modules.append_row( literal_module_main ).result;
      module_global = modules.append_row( literal_module_global ).result;
    }

    void add_constant(sys::literal name, type_system::base::i_value * value, configuration::module_config * h_module)
    {
      value->into(
        &h_module->constants.append_row( name, std::make_unique<execution::sequence>() ).result->value
      );
    }
  };

  struct vm_config
  {
    vm_config_settings                              settings;
    type_system::info::static_data                  static_information{ &settings };
  };

}
namespace ksi::interpreter::type_system::info
{

  hints::type_pointer  internal::reg_type(params & p, sys::sview name, std::initializer_list<hints::cat_pointer> cats)
  {
    hints::type * tp = p.type_table.append_row(
      p.from->dict->add(sys::string{name}).pointer
    ).result;
    tp->relate_to_cats.insert_range(cats);
    p.from->add_constant(tp->name, tp, p.from->module_global);
    return tp;
  }

  hints::cat_pointer  internal::reg_cat(params & p, sys::sview name)
  {
    hints::cat_pointer hc = p.category_table.append_row(
      p.from->dict->add(sys::string{name}).pointer
    ).result;
    p.from->add_constant(hc->name, hc, p.from->module_global);
    return hc;
  }

}
namespace ksi::interpreter::execution
{

  struct thread
  {
    vm_config                 * h_config;
    std::vector<module_data>  modules;
    data::call_stack          call_stack;
    data::stack               stack;

    void add_modules()
    {
      modules.reserve( h_config->settings.modules.pos.size() );
      for( configuration::module_config * module_configuration : h_config->settings.modules.pos )
      {
        modules.emplace_back(module_configuration);
      }
    }

    module_data * get_module(size_t index)
    {
      if( modules.size() < h_config->settings.modules.pos.size() ) { add_modules(); }

      return &modules[index];
    }

    thread(vm_config * h) : h_config{ h }
    {
      add_modules();
      stack.add_frame(20);
    }

    void run(params & p)
    {
      module_data * md = get_module(h_config->settings.module_main->id);
      type_system::base::i_value * value = md->find_value(h_config->settings.literal_do, p);
      sequence * seq = sequence::from_value(value, p.vm_configuration->static_information);
      if( seq == nullptr ) { return; }

      call_stack.run(seq, p);
    }
  };

}
