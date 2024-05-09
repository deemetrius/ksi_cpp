#pragma once

#include "type_system.hpp"

namespace ksi::interpreter
{

  struct vm_config;

}
namespace ksi::interpreter::configuration
{
  struct sequence;
}
namespace ksi::interpreter::execution
{

  using seq_pointer = configuration::sequence const *;

  struct params
  {
    //log_pointer log;
    vm_config * vm_configuration;
  };

  struct instruction
  {
    struct data_type
    {
    };

    using data_pointer = const data_type *;

    static void empty_function(params & p, data_pointer d) {}

    using function = decltype( &empty_function );

    data_pointer  data;
    function      fn{ &empty_function };

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

  struct module_config : type_system::meta::meta_information
  {
    struct variable_information
    {
      sys::literal                          name;
      type_system::hints::i_hint            * type_restriction;
      std::size_t                           position;
      sys::unique<configuration::sequence>  seq_init;

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

    sys::static_table<variable_information> variables;
    sys::static_table<read_only_cell>       constants;

    module_config(type_system::meta::meta_information info) : type_system::meta::meta_information{ info } {}
  };

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
    using stack_frame = sys::unique<type_system::impl::impl_array>;

    std::list<stack_frame> frames;
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
