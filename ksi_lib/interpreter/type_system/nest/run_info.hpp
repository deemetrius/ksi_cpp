#pragma once

#include "runtime/space_data.hpp"
#include "runtime/stack_values.hpp"
#include "runtime/call_stack.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::run_data
  {
    // props
    runtime::space_data thread_space;
  };


  template <typename Type_config>
  struct system<Type_config>::run_info
    : public system<Type_config>::runtime::run_data
    , public system<Type_config>::runtime::stack_values
    , public system<Type_config>::runtime::call_stack
  {};


} // ns
