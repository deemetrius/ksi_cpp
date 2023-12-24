#pragma once

#include <vector>
#include <map>
#include <memory>
#include "iterator_random_access.hpp"

namespace ksi::lib {

  template <typename String>
  struct dict
  {
    using size_type = std::ptrdiff_t;
    using string_type = String;

    struct node
    {
      string_type name;
      size_type index;
      size_type key;
    };

    using value_type = node;
    using reference = node &;

    using nodes_type = std::vector<node>;
    using map_type = std::map<string_type, size_type>;

    struct storage
    {
      nodes_type nodes;
      map_type map;
    };

    using data_type = std::unique_ptr<storage>;

  protected:
    data_type data;

  public:

    dict()
      : data{ std::make_unique<storage>() }
    {}


    struct it_data
    {
      using difference_type = size_type;
      using value_type = node;
      using pointer = node *;
      using reference = node &;

      using ordering = std::strong_ordering;


      storage * handle;
      size_type index;


      constexpr reference access() const
      {
        return handle->nodes[index];
      }

      constexpr pointer get() const
      {
        return &handle->nodes[index];
      }


      static constexpr it_data advance(const it_data & it, difference_type n)
      {
        return {it.handle, it.index + n};
      }

      static constexpr it_data recede(const it_data & it, difference_type n)
      {
        return {it.handle, it.index - n};
      }


      static constexpr difference_type difference(const it_data & it1, const it_data & it2)
      {
        return (it1.index - it2.index);
      }

      static constexpr ordering compare(const it_data & it1, const it_data & it2)
      {
        return (it1.index <=> it2.index);
      }
    };

    using iterator = iterator_random_access<it_data, direction::n_forward>;
    using range_type = is_range<iterator>;

    range_type get_range()
    {
      return {
        {data.get(), 0},
        {data.get(), std::ssize(data->nodes)}
      };
    }


    iterator try_add(string_type name)
    {
      typename map_type::iterator it = data->map.find(name);
      if( it != data->map.end() )
      {
        return {data.get(), it->second};
      }

      size_type index = std::ssize(data->nodes);
      it = data->map.lower_bound(name);
      size_type key = (( it != data->map.end() ) ? data->nodes[it->second].key : index );

      data->nodes.emplace_back(name, index, -1);
      it = data->map.try_emplace(name, index).first;
      reindex(it, key);

      return {data.get(), it->second};
    }

    void reindex(typename map_type::iterator it_from, size_type from_key)
    {
      for( typename map_type::value_type & item : is_range<typename map_type::iterator>{it_from, data->map.end()} )
      {
        data->nodes[item.second].key = from_key;
        ++from_key;
      }
    }
  };

} // end ns
