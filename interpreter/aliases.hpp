#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "lib/msg.hpp"

#include "lib/dict.hpp"
#include "lib/table.hpp"
#include "lib/unique.hpp"
#include "lib/unique_inner.hpp"

namespace ksi::interpreter::sys
{

  using integer = std::intptr_t;
  using string = std::string;
  using character = string::value_type;
  using sview = std::basic_string_view<character>;

  using dictionary = lib::dict<string>;
  using literal = dictionary::map_type::const_pointer;

  template <typename T>
  using static_table = lib::table<T, &T::name, typename T::Less>;

  using lib::unique;
  using lib::unique_inner;

}
namespace ksi::interpreter::detail
{

  struct literal_less
  {
    bool operator () (sys::literal lt, sys::literal rt) const
    {
      return (lt->second.value < rt->second.value);
    }
  };

}
namespace ksi::interpreter
{
  using lib::msg;
  using lib::error_msg;
}
