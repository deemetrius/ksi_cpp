#pragma once

#include "types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::value
  {
    virtual ptr_type get_type(ptr_system_types sys_types) const = 0;
  };


} // ns
