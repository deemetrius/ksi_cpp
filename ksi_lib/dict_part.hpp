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
    {pointer_to<const dict_record>, part_index, true}
*/

namespace ksi::lib {


  template <typename String>
  struct dict_part
  {
    using dict_type = dict<String>;
    using dict_value_type = dict_type::value_type;
    using dict_iterator = dict_type::iterator;

    using ptr_dict_type = std::shared_ptr<dict_type>;

    using const_pointer = dict_type::const_pointer;
    using index_type = dict_index_type;

    struct less
    {
      constexpr bool operator () (const_pointer k1, const_pointer k2) const
      {
        return (k1->id < k2->id);
      }
    };

    using map_type = std::map<const_pointer, index_type, less>;
    using map_iterator = map_type::iterator;

    struct result_has
    {
      enum status_type { not_in_dict, part_missing, term_present };

      const_pointer value;
      index_type    index;
      status_type   status;

      constexpr bool included() const { return (status == term_present); }
    };

    struct result_add
    {
      const_pointer value;
      index_type index;
      bool was_added;
    };

    using term_type = dict_type::term_type;
    using term_view_type = dict_type::term_view_type;

    // props
    ptr_dict_type dict_pointer;
    map_type map;

    // actions

    result_has has(term_view_type term)
    {
      auto [dict_it, dict_not_has] = dict_pointer->has(term);
      if( dict_not_has ) { return {nullptr, 0, result_has::not_in_dict}; }

      map_iterator it = map.find(dict_it);
      if( it == map.end() ) { return {dict_it->get_const(), 1, result_has::part_missing}; }

      return {it->first, it->second, result_has::term_exists};
    }

    result_has has(const_pointer dict_value)
    {
      map_iterator it = map.find(dict_value);
      if( it == map.end() ) { return {dict_value, 1, result_has::part_missing}; }

      return {it->first, it->second, result_has::term_present};
    }

    result_add add(term_type term)
    {
      dict_iterator dict_it = dict_pointer->add( std::move(term) ).it;
      auto [it, was_added] = map.try_emplace( dict_it->get_const(), map.size() );
      return {it->first, it->second, was_added};
    }
  };


} // ns
