#pragma once

  #include "dict.cases.hpp"
  #include "concepts.hpp"

  #include <string_view>
  #include <memory>

  #include <vector>
  #include <map>

  #include <utility>
  #include <optional>

namespace ksi::lib {


  template <typename String>
  struct dict
  {
    using id_type   = dict_id_type;

    using term_type = String;
    using term_key  = std::basic_string_view<typename term_type::value_type>;
    using term_ptr  = std::unique_ptr<term_type>;

    using rank_type = dict_rank_type;

    struct value_type
    {
      id_type   id;
      term_ptr  term;
      rank_type rank;
    };

    using data = std::vector<value_type>;
    using road = std::map<term_key, id_type>;
    using road_iterator = road::iterator;

    struct result_type
    {
      id_type id;
      bool was_added;
    };

    // props
    data values;
    road map;

    // actions

    result_type has(term_key term) const
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
        auto [id, map_key] = traits::add_value( this, upper, std::move(term) );
        road_iterator it = traits::add_to_map(this, upper, map_key, id);
        traits::rank_recalc_after(this, it);

        return {id, true};
      }
      return {lower->second, false};
    }

  private:

    struct traits
    {
      using road_iterator = road::iterator;
      using road_iterator_const = road::const_iterator;
      using pointer = dict *;
      using const_pointer = const dict *;

      struct result_add_value
      {
        id_type   id;
        term_key  map_key;
      };

      static result_add_value add_value(pointer self, road_iterator_const hint, term_type term)
      {
        id_type id{ self->values.size() };
        rank_type rank{ rank_prev(self, hint) + 1 };
        return {id, *(self->values.emplace_back(
          id,
          std::make_unique<term_type>( std::move(term) ),
          rank
        ).term)};
      }

      static road_iterator add_to_map(pointer self, road_iterator_const hint, term_key map_key, id_type id)
      {
        road_iterator it;

        std::optional<errors::dict_params::map_mismatch> map_mismatch;
        try
        {
          it = self->map.try_emplace(hint, map_key, id);
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

        return it;
      }

      static value_type & get(pointer self, ksi::concepts::iterator_of<road> auto it)
      {
        return self->values[it->second];
      }

      static value_type const & get_const(const_pointer self, ksi::concepts::iterator_of<road> auto it)
      {
        return self->values[it->second];
      }

      static rank_type rank_prev(pointer self, ksi::concepts::iterator_of<road> auto it)
      {
        return (
          (self->map.begin() != it) ? (
            (self->map.end() != it) ?
            get_const(self, --it).rank :
            get_const( self, self->map.crbegin() ).rank
          ) :
          0
        );
      }

      static void rank_recalc_after(pointer self, road_iterator it)
      {
        road_iterator it_end{ self->map.end() };
        rank_type rank{ get_const(self, it).rank };
        while( (++it) != it_end )
        {
          get(self, it).rank = (++rank);
        }
      }
    };
  };


} // ns
