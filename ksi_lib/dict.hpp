#pragma once

  #include <vector>
  #include <map>

  #include <string>
  #include <string_view>

  #include <utility>
  #include <optional>

namespace ksi::lib {


  using dict_id_type = std::size_t;
  using dict_rank_type = std::size_t;


} // ns
namespace ksi::lib::errors::dict_params {


  struct vector_only
  {
    dict_id_type  vector_id;

    void throw_exception();
  };

  struct map_mismatch
  {
    dict_id_type  vector_id, map_id;

    void throw_exception();
  };


} // ns
namespace ksi::lib::errors {


  struct base
  {
    std::string_view msg;
  };


  struct dict_inconsistent {};

  struct dict_inconsistent_vector_only
    : public dict_params::vector_only
    , public base
    , public dict_inconsistent
  {};

  struct dict_inconsistent_map_mismatch
    : public dict_params::map_mismatch
    , public base
    , public dict_inconsistent
  {};


} // ns
namespace ksi::lib::errors::dict_params {

  using namespace std::string_view_literals;


  void vector_only::throw_exception()
  {
    throw dict_inconsistent_vector_only{*this, "dict::add() ~ Unable to create map's record"sv};
  }


  void map_mismatch::throw_exception()
  {
    throw dict_inconsistent_map_mismatch{*this, "dict::add() ~ map already contains record with wrong id"sv};
  }


} // ns
namespace ksi::lib {


  template <typename String>
  struct dict
  {
    using term_type = String;
    using id_type   = dict_id_type;
    using rank_type = dict_rank_type;


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

        std::optional<errors::dict_params::map_mismatch> map_mismatch;
        road_iterator it;
        try
        {
          it = map.try_emplace(upper, term, id);
          if( id != it->second )
          {
            map_mismatch.emplace(id, it->second);
          }
        }
        catch( ... )
        {
          errors::dict_params::vector_only{id}.throw_exception();
        }

        if( map_mismatch.has_value() )
        {
          map_mismatch->throw_exception();
        }

        rank_recalc_after(it);
        return {id, true};
      }
      return {lower->second, false};
    }

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
