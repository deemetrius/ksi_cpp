#pragma once

#include "../types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::junction
  {
    // props
    in_junction_map from_points;
  };


} // ns
