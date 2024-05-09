#pragma once

  #include <concepts>
  #include <algorithm>

  #include <cstdint>

namespace ksi::lib {


  template <typename Base, std::derived_from<Base> ... Classes>
  struct keep
  {
    static constexpr std::size_t max_size{ std::max({ sizeof(Classes) ... }) };
    static constexpr std::size_t max_align{ std::max({ alignof(Classes) ... }) };

    struct storage
    {
      enum class byte : std::uint8_t {};

      alignas(max_align) byte data[max_size];
    };

    storage inner;
    Base * handle;
  };


} // ns
