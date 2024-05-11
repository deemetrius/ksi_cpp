#pragma once

#include <cstddef>

namespace ksi::lib
{

  template <std::size_t N>
  struct static_text
  {
    char text[N];

    constexpr static_text(const char (&str)[N])
    {
      for(std::size_t i = 0; i < N; ++i)
      {
        text[i] = str[i];
      }
    }
  };

  namespace static_text_literal
  {

    template <static_text st>
    constexpr decltype(st) operator ""_st ()
    {
      return st;
    }

  }

}