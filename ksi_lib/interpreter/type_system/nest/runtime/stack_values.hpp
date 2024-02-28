#pragma once

#include "../values/value_array.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::stack_values
  {
    // props
    values::value_array data{25, care::is_root};
  };


} // ns
