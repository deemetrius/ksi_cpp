#pragma once

  #include "concepts.hpp"

namespace ksi::lib {


  template <typename Type, concepts::is<Type> ... Args>
  constexpr bool one_of(Type value, std::type_identity_t<Type> first, Args ... rest)
  {
    return ((value == first) || ((value == rest) || ...));
  }


} // ns
