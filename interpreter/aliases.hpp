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
  using namespace std::string_literals;

  using lib::msg;
  using lib::error_msg;

  struct parser_result
  {
    sys::string name;
    std::size_t line;
  };

  struct src_info
  {
    std::size_t   line;
    sys::literal  name;
  };

  struct var_info
  {
    std::size_t   id;
  };

  struct meta_info : src_info, var_info
  {
    static inline const auto auto_increment{ &meta_info::id };

    using Less = detail::literal_less;
  };

  template <typename T, typename Base = T>
  using static_table = lib::table<T, Base, &T::name, typename T::Less>;
}
