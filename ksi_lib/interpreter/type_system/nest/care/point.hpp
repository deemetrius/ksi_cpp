#pragma once

#include "../types_nest.hpp"
#include <set>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::point
  {
    using cells_type = std::set<ptr_cell>;

    // props
    cells_type from_cells;
    bool is_root;
  };


} // ns
