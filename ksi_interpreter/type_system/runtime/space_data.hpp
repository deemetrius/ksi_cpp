#pragma once

#include "system_types.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::space_data
  {
    // props
    system_types sys_types;
  };


} // ns
