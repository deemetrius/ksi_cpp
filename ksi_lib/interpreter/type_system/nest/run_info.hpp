#pragma once

#include "values/value_array.hpp"
#include "execution/stack_values.hpp"
#include "execution/call_stack.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::execution::space_data
  {};


  template <typename Type_config>
  struct types<Type_config>::execution::run_data
  {
    // props
    execution::space_data thread_space;
  };


  template <typename Type_config>
  struct types<Type_config>::run_info
    : public types<Type_config>::execution::run_data
    , public types<Type_config>::execution::stack_values
    , public types<Type_config>::execution::call_stack
  {};


} // ns
