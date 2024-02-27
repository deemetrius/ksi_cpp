#pragma once

#include "instructions.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::execution::sequence
  {
    using isntr_groups = std::vector<instructions::group_type>;

    // props
    instructions::position  self_position;
    isntr_groups            instruction_groups;
    //var_names
  };


} // ns
