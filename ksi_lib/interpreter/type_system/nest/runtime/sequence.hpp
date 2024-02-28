#pragma once

#include "execution.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::sequence
  {
    using isntr_groups = std::vector<execution::group_type>;

    // props
    execution::position  self_position;
    isntr_groups            instruction_groups;
    //var_names
  };


} // ns
