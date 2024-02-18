#pragma once

#include "../types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::slot
  {
    // props
    ptr_cell cell_handle;
    ptr_point point_handle;
  };


} // ns
