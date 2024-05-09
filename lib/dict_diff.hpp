#pragma once

#include "dict.hpp"
#include <memory>

namespace ksi::lib
{

  template <typename String>
  struct dict_diff
  {
    using dict_type = dict<String>;
    using pointer = typename dict_type::map_type::pointer;

    std::shared_ptr<dict_type>  original;
    dict_type                   difference{ .id_initial = original->storage.size() };

    pointer find(String const & key)
    {
      pointer ret = original->find();
      return ((ret == nullptr) ? difference.find(key) : ret);
    }

    pointer add(String literal)
    {
      pointer pt = original->find(literal);
      return ((pt == nullptr) ? difference.add( std::move(literal) ).pointer : pt);
    }
  };

}