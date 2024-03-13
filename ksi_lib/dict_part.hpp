#pragma once

  #include "dict.hpp"
  #include <memory>
  #include <map>

/*
dict_part::has(term) ~ returns:
  if term is absent in dict_part then:
    when source dict has term - returns _ its record pointer as .value; .index = 1; .was_added{ fasle }
    otherwise ~ {nullptr, 0, false}
  else: (term exists in dict_part)
    {pointer to dict_record_const, part_index, true}
*/

namespace ksi::lib {


  template <typename String>
  struct dict_part
  {
    using dict_type = dict<String>;
    using dict_value_type = dict_type::value_type;
    using dict_iterator = dict_type::iterator;

    using ptr_dict_type = std::shared_ptr<dict_type>;

    using map_key_type = dict_value_type const *;
    using index_type = dict_index_type;

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

    using term_type = dict_type::term_type;
    using term_view_type = dict_type::term_view_type;

    // props
    ptr_dict_type dict_pointer;
    map_type map;

    // actions

    result_type has(term_view_type term)
    {
      auto [dict_it, dict_has] = dict_pointer->has(term);
      if( ! dict_has ) { return {nullptr, 0, false}; }

      map_key_type dict_value = dict_it->get_const();
      map_iterator it = map.find(dict_value);
      if( it == map.end() ) { return {dict_value, 1, false}; }

      return {it->first, it->second, true};
    }
  };


} // ns
