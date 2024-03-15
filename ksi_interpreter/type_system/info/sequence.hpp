#pragma once

#include "../nest/execution.hpp"
#include "var_names.hpp"

/*
  сиквенс не хранит значения переменных,
  а лишь список имён переменных и констант

  значения же будут в ~ runtime::call_stack > runtime::sequence_space > stack_frame
*/  // и возможно ещё значения в ~ thread_space > static_data

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::info::sequence
  {
    // props
    execution::position       self_position;
    execution::isntr_groups   instruction_groups;
    info::var_names           variables;

    // actions

    execution::ptr_instruction_const instruction_get_pointer(execution::position) const
    {
      return &( instruction_groups[pos.group_index][pos.instr_index] );
    }
  };


} // ns
