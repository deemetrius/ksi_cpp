#pragma once

#include "../system_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::care::junction
  {
    enum class result_of_entraining { was_increased, was_added };
    enum class result_of_detraining { was_decreased, was_removed, was_absent, was_wrong_count };

    // props
    in_junction_map map_points_counts;

    bool refs_empty() const
    {
      return map_points_counts.empty();
    }

    result_of_entraining refs_entrain(ptr_point source_point)
    {
      if( typename in_junction_map::iterator it = map_points_counts.find(source_point); it != map_points_counts.end() )
      {
        ++it->second;
        return result_of_entraining::was_increased;
      }
      map_points_counts.try_emplace(source_point, 1);
      return result_of_entraining::was_added;
    }

    result_of_detraining refs_detrain(ptr_point source_point)
    {
      if( typename in_junction_map::iterator it = map_points_counts.find(source_point); it != map_points_counts.end() )
      {
        if( it->second > 1 )
        {
          --it->second;
          return result_of_detraining::was_decreased;
        }
        else if( it->second == 1 )
        {
          map_points_counts.erase(it);
          return result_of_detraining::was_removed;
        }
        else
        {
          return result_of_detraining::was_wrong_count;
        }
      }
      else
      {
        return result_of_detraining::was_absent;
      }
    }
  };


} // ns
