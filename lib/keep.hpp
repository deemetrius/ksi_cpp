#pragma once

  #include <concepts>
  #include "max.hpp"

namespace ksi::lib {


  template <typename Base, std::derived_from<Base> ... Classes>
  struct keep
  {
    static constexpr std::size_t max_size{ max(sizeof(Classes) ...) };
    static constexpr std::size_t max_align{ max(alignof(Classes) ...) };

    struct storage
    {
      alignas(max_align) char data[max_size];
    };

    storage inner;
    Base * handle;

    Base * get() { return reinterpret_cast<Base *>(& inner.data); }
  };


} // ns
