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

    template <typename T>
    T get() const
    {
      if constexpr( std::is_same_v<T, bool> )
      {
        return this->boolean;
      }
      else
      {
        static_assert("This type is not supported");
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
      std::size_t                           position;
      sys::unique<execution::sequence>      seq_init;

      static inline const auto auto_increment{ &variable_information::position };

      using Less = detail::literal_less;
    };

    struct read_only_cell
    {
      sys::literal                    name;
      type_system::brick::cell_type   value;
      std::size_t                     position;

      static inline const auto auto_increment{ &read_only_cell::position };

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

      if( variables->elements.size() < var_info->position )
      {
        add_more_variables();
      }

      return get_value(var_info->position, p);
    }
  };

}
namespace ksi::interpreter
{

  struct vm_config
  {
    std::shared_ptr<sys::dictionary>                dict = std::make_shared<sys::dictionary>();
    type_system::info::static_data                  static_information{ dict.get() };
    static_table<configuration::module_config> modules;

    configuration::module_config * module_main;

    sys::literal literal_main_module{ dict->add(sys::string{"@main"sv}).pointer };
    sys::literal literal_do{ dict->add(sys::string{"do"sv}).pointer };

    vm_config()
    {
      module_main = modules.append_row( literal_main_module ).result;
    }
  };

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
      modules.reserve( h_config->modules.pos.size() );
      for( configuration::module_config * module_configuration : h_config->modules.pos )
      {
        modules.emplace_back(module_configuration);
      }
    }

    module_data * get_module(size_t index)
    {
      if( modules.size() < h_config->modules.pos.size() ) { add_modules(); }

      return &modules[index];
    }

    thread(vm_config * h) : h_config{ h }
    {
      add_modules();
      stack.add_frame(20);
    }

    void run(params & p)
    {
      module_data * md = get_module(h_config->module_main->id);
      type_system::base::i_value * value = md->find_value(h_config->literal_do, p);
      sequence * seq = sequence::from_value(value, p.vm_configuration->static_information);
      if( seq == nullptr ) { return; }

      call_stack.run(seq, p);
    }
  };

}
