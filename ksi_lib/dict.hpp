#pragma once

#include <vector>
#include <map>

namespace ksi::lib {


  template <typename String>
  struct dict
  {
    using term_type = String;
    using id_type   = std::size_t;
    using rank_type = std::size_t;

    struct value_type
    {
      term_type term;
      id_type   id;
      rank_type rank;
    };

    using data = std::vector<value_type>;
    using road = std::map<term_type, id_type>;
    using road_iterator = road::iterator;

    // props
    data values;
    road map;

    struct result_type
    {
      id_type id;
      bool was_added;
    };

    result_type has(term_type term) const
    {
      typename road::const_iterator it{ map.find(term) };
      return ((map.end() == it) ? result_type{0, false} : result_type{it->second, true});
    }

    result_type add(term_type term)
    {
      id_type id{ values.size() };
      auto [it, was_added] = map.try_emplace(term, id);
      if( was_added ) try
      {
        rank_type rank{ rank_prev(it) + 1 };
        values.emplace_back(term, id, rank);
        rank_recalc_after(it);
      }
      catch( ... )
      {
        map.erase(it);
        throw;
      }
      return {it->second, was_added};
    }

    value_type const & get_const(road_iterator it) const
    {
      return values[it->second];
    }

    value_type & get(road_iterator it)
    {
      return values[it->second];
    }

  private:
    rank_type rank_prev(road_iterator it)
    {
      return ((map.begin() != it) ? get_const(--it).rank : 0);
    }

    void rank_recalc_after(road_iterator it)
    {
      road_iterator it_end{ map.end() };
      rank_type rank{ get_const(it).rank };
      while( (++it) != it_end )
      {
        get(it).rank = (++rank);
      }
    }
  };


} // ns
