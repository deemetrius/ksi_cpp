#pragma once

#include "../nest/execution.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::sequence
  {
    using isntr_groups = std::vector<execution::group_type>;

    // props
    execution::position   self_position;
    isntr_groups          instruction_groups;
    //var_names

    // actions

    execution::ptr_instruction_const instruction_get_pointer(execution::position) const
    {
      return &( instruction_groups[pos.group_index][pos.instr_index] );
    }
  };


} // ns
