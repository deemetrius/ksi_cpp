#pragma once

  #include "dict.hpp"
  #include <memory>
  #include <map>

namespace ksi::lib {


  template <typename String>
  struct dict_part
  {
    using dict_type = dict<String>;
    using dict_value_type = dict_type::value_type;
    using map_key_type = dict_value_type const *;
    using ptr_dict_type = std::shared_ptr<dict_type>;

    using index_type = dict_index_type;

    using term_view_type = dict_type::term_view_type;

    struct less
    {
      constexpr bool operator () (map_key_type k1, map_key_type k2) const
      {
        return (k1 < k2);
      }
    };

    using map_type = std::map<map_key_type, index_type, less>;
    using map_iterator = map_type::iterator;

    struct result_type
    {
      map_key_type value;
      index_type index;
      bool was_added;
    };

    // props
    ptr_dict_type dict_pointer;
    map_type map;

    // actions

    result_type has(term_view_type term)
    {
      auto [dict_it, dict_has] = dict_pointer->has(term);
      if( ! dict_has ) { return {nullptr, 0, false}; }

      map_key_type dict_value{ dict_it->get_const() };
      map_iterator it = map.find(dict_value);
      if( it == map.end() ) { return {dict_value, 1, false}; }

      return {it->first->get_const(), it->second, true};
    }
  };


} // ns
