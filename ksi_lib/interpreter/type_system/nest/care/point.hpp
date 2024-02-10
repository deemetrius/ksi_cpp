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

    bool rel_add(ptr_cell from_cell)
    {
      return from_cells.insert(from_cell)->second;
    }

    bool rel_del(ptr_cell from_cell)
    {
      return (from_cells.erase(from_cell) > 0);
    }
  };


} // ns
