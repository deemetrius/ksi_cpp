#pragma once

#include "../types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::point
  {
    // props
    in_point_set from_cells;
    bool is_root;

    bool rels_empty() const
    {
      return from_cells.empty();
    }

    bool rels_only_one() const
    {
      return (from_cells.size() == 1);
    }

    bool rel_add(ptr_cell from_cell)
    {
      return from_cells.insert(from_cell).second;
    }

    bool rel_del(ptr_cell from_cell)
    {
      return (from_cells.erase(from_cell) > 0);
    }
  };


} // ns
