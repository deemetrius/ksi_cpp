#pragma once

  #include <type_traits>
  #include "concepts/is.hpp"

namespace ksi::lib {


  template <typename T>
  constexpr T max(T first)
  {
    return first;
  }

  template <typename T, concepts::is<T> ... Args>
  constexpr T max(T first, std::type_identity_t<T> second, Args ... rest)
  {
    if constexpr( sizeof...(rest) > 0 )
    {
      return ((first < second) ? max(second, rest ...) : first);
    } else {
      return ((first < second) ? second : first);
    }
  }


} // ns
