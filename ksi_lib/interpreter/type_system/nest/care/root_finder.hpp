#pragma once

#include "point.hpp"
#include "junction.hpp"
#include "../walker.hpp"
#include <forward_list>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::root_finder
  {
    using points_set = std::set<ptr_point>;
    using cells_set = std::set<ptr_cell>;
    using point_walker = ksi::lib::walker<typename in_point_set::iterator>;
    using point_squad_walkers = std::forward_list<point_walker>;

    // props
    points_set traversed_points;
    cells_set traversed_cells;
    // ? should i store here path to root ?

    void add_walker(point_squad_walkers & to_squad, ptr_point for_point)
    {
      to_squad.emplace_front( for_point->from_cells.begin(), for_point->from_cells.end() );
      traversed_points.insert(for_point);
    }

    bool find(ptr_point for_point)
    {
      point_squad_walkers squad;
      add_walker(squad, for_point);
      for( point_walker * walker{ nullptr }; ! squad.empty(); (walker != nullptr) && walker->advance(); )
      {
        point_walker * walker = squad.begin();

        if( walker->is_end() )
        {
          walker = nullptr;
          squad.pop_front();
          continue;
        }

        ptr_cell cell_handle{ *walker->current };
        if( traversed_cells.contains(cell_handle) )
        {
          continue;
        }
        traversed_cells.insert(cell_handle);

        for( typename in_junction_map::const_reference it : cell_handle->junction_point.map_points_counts )
        {
          if( it.first->is_root )
          {
            // seems this check is supposed to be after: if traversed_points.contains()
            // please forgive me for such small-optimization
            return true;
          }
          if( traversed_points.contains(it.first) )
          {
            continue;
          }
          add_walker(squad, it.first);
        }
      }
      while( ! squad.empty() );
      return false;
    }
  };


} // ns
