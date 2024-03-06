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
    using id_type = size_type;
    using string_type = String;

    struct node
    {
      string_type   term;
      id_type       id;
      size_type     value;
    };

    using value_type = node;
    using reference = node &;

    using nodes_type = std::vector<node>;
    using map_type = std::map<string_type, id_type>;

    struct storage
    {
      nodes_type  nodes;
      map_type    map;
    };

    using ptr_storage = storage *;
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

      // props
      ptr_storage handle;
      size_type   index;


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


    bool contains(string_type term) const
    {
      return ( data->map.find(term) != data->map.end() );
    }


    struct result_try_add
    {
      iterator  it;
      bool      was_added;
    };


    result_try_add try_add(string_type term)
    {
      if(
        typename map_type::iterator it = data->map.find(term);
        it != data->map.end()
      ) {
        return {{data.get(), it->second}, false};
      }

      id_type id = std::ssize(data->nodes);
      typename map_type::iterator it_near = data->map.lower_bound(term);
      size_type value{ ( it_near != data->map.end() ) ? data->nodes[it_near->second].value : id };

      auto [it, was_added] = data->map.try_emplace(term, id);
      if( was_added )
      {
        data->nodes.emplace_back(term, id, -1);
        reindex(it, value);
      }
      return {{data.get(), it->second}, was_added};
    }

    void reindex(typename map_type::iterator it_from, size_type value_start)
    {
      for( typename map_type::value_type & item : is_range<typename map_type::iterator>{it_from, data->map.end()} )
      {
        data->nodes[item.second].value = value_start;
        ++value_start;
      }
    }
  };

} // end ns
