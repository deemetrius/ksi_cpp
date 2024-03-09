#pragma once

#include <vector>
#include <map>
#include <utility>
#include <optional>

namespace ksi::lib {


  using dict_id_type = std::size_t;


} // ns
namespace ksi::lib::errors::dict_params {


  struct vector_only
  {
    dict_id_type  vector_id;
  };

  struct map_mismatch
  {
    dict_id_type  vector_id, map_id;
  };


} // ns
namespace ksi::lib::errors {


  struct dict_inconsistent {};

  struct dict_inconsistent_vector_only
    : public dict_params::vector_only
    , public dict_inconsistent
  {};

  struct dict_inconsistent_map_mismatch
    : public dict_params::map_mismatch
    , public dict_inconsistent
  {};


} // ns
namespace ksi::lib {


  template <typename String>
  struct dict
  {
    using term_type = String;
    using id_type   = dict_id_type;
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
    using road_iterator_const = road::const_iterator;

    // props
    data values;
    road map;


    struct result_type
    {
      id_type id;
      bool was_added;
    };

    // actions

    result_type has(term_type term) const
    {
      typename road::const_iterator it{ map.find(term) };
      return (
        (map.end() == it) ?
        result_type{0, false} :
        result_type{it->second, true}
      );
    }

    result_type add(term_type term)
    {
      auto [lower, upper] = std::as_const(map).equal_range(term);
      if( lower == upper )
      {
        id_type id{ values.size() };
        rank_type rank{ rank_prev(upper) + 1 };
        values.emplace_back(term, id, rank);

        std::optional<errors::dict_inconsistent_map_mismatch> map_mismatch;
        road_iterator it;
        try
        {
          it = map.try_emplace(upper, term, id);
          if( id != it->second )
          {
            map_mismatch = errors::dict_inconsistent_map_mismatch{id, it->second};
          }
        }
        catch( ... )
        {
          throw errors::dict_inconsistent_vector_only{id};
        }

        if( map_mismatch.has_value() )
        {
          throw map_mismatch.value();
        }

        rank_recalc_after(it);
        return {id, true};
      }
      return {lower->second, false};
    }

    /* result_type add(term_type term)
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
    } */

    value_type & get(road_iterator it)
    {
      return values[it->second];
    }

    value_type const & get_const(auto it) const
    {
      return values[it->second];
    }

    rank_type rank_prev(road_iterator_const it)
    {
      return (
        (map.begin() != it) ? (
          (map.end() != it) ?
          get_const(--it).rank :
          get_const( map.crbegin() ).rank
        ) :
        0
      );
    }

  private:
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
