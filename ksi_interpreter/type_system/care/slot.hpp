#pragma once

#include "../system_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::care::slot
  {
    // props
    ptr_cell                  cell_handle;
    ptr_point                 point_handle;
    info::ptr_property_info   meta_information{ nullptr };
  };


} // ns
