#pragma once

/*
  value_module: // runtime part (thread_space)
    properties // base : value_array
    self_config

  constant:
    name
    value
*/

#include "bases/value_managed.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::values::value_module
    : public system<Type_config>::bases::value_managed
  {
    //
  };


} // ns
