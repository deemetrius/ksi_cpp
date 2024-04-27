#pragma once

  #include <map>

namespace ksi::lib {


  template
  <typename String>
  struct dict
  {
    struct data
    {
      std::intptr_t id, value;
    };

    using map_type = std::map<String, data>;

    map_type storage;

    struct insert_result
    {
      bool was_there;
      typename map_type::iterator iterator;

      typename map_type::pointer get_pointer() const
      { return &*iterator; }
    };

    dict() { storage.emplace_hint(storage.end(), String{}, data{0, 0}); }

    insert_result add(String literal)
    {
      auto [it, was_added] = storage.try_emplace(std::move(literal), data{static_cast<std::intptr_t>( storage.size() )});

      //reindex
      std::intptr_t value = 0;
      if( storage.begin() != it ) { value = (std::ranges::prev(it)->second.value + 1); }
      for( typename map_type::iterator cur = it; cur != storage.end(); ++cur )
      {
        cur->second.value = ++value;
      }

      return {!was_added, it};
    }
  };


}
