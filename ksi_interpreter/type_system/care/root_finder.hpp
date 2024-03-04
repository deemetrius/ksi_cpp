#pragma once

#include "point.hpp"
#include "cell.hpp"
#include "ksi_lib/walker.hpp"
#include <forward_list>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::care::root_finder
  {
    using points_set = std::set<ptr_point>;
    using cells_set = std::set<ptr_cell>;
    using point_walker = ksi::lib::walker<typename in_point_set::iterator>;
    using point_squad_walkers = std::forward_list<point_walker>;

    // props
    points_set traversed_points;
    cells_set traversed_cells;
    // ? should i store here path to root ?

    bool is_cell_rooted(ptr_cell cell_handle)
    {
      if( traversed_cells.insert(cell_handle).second == false ) { return false; }
      for( typename in_junction_map::const_reference it : cell_handle->junction_point.map_points_counts )
      {
        if( is_point_rooted(it.first) ) { return true; }
      }
      return false;
    }

    bool is_point_rooted(ptr_point point_handle)
    {
      if( traversed_points.insert(point_handle).second == false ) { return false; }
      point_squad_walkers squad;
      squad.emplace_front( point_handle->from_cells.begin(), point_handle->from_cells.end() );

      for( point_walker * walker{ nullptr }; ! squad.empty(); (walker != nullptr) ? walker->advance() : void() )
      {
        walker = & squad.front();

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

  protected:
    void add_walker(point_squad_walkers & to_squad, ptr_point point_handle)
    {
      to_squad.emplace_front( point_handle->from_cells.begin(), point_handle->from_cells.end() );
      traversed_points.insert(point_handle);
    }
  };


} // ns
