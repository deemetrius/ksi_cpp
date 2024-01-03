#pragma once

#include "../dict.hpp"

namespace ksi::interpreter {

  template <typename String>
  struct vars
  {
    using size_type = std::ptrdiff_t;
    using string_type = String;
    using dict_type = ksi::lib::dict<String>;
    using entry_type = dict_type::iterator;
    using names_type = std::vector<entry_type>;
    using map_type = std::map<size_type, size_type>; // entry->value : index

    size_type add(entry_type entry)
    {
      typename map_type::iterator it = map.find(entry->value);
      if( it != map.end() ) { return -1; }
      return inner_add(entry)
    }

    size_type try_add(entry_type entry)
    {
      typename map_type::iterator it = map.find(entry->value);
      if( it != map.end() ) { return it->second; }
      return inner_add(entry)
    }

    entry_type const & operator [] (size_type index) const
    {
      return names[index];
    }

  protected:
    names_type names;
    map_type map;

    size_type inner_add(entry_type entry)
    {
      size_type index = std::ssize(names);
      map.try_emplace( entry->value, index);
      names.push_back(entry);
      return index;
    }
  };

} // ens ns
