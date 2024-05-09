#pragma once

#include <print>
#include <cstdio>

#include <string_view>
#include <string>


namespace ksi::lib
{

  using message = std::string_view;

  template <std::size_t N>
  message make_message(const char (& str)[N])
  {
    std::size_t count{ N - 1 };
    bool is_zero_end = (str[count] == '\0');
    return message{ str, is_zero_end ? count : N };
  }

  template <std::size_t N>
  void msg(const char (& str)[N])
  {
    std::size_t count{ N - 1 };
    bool is_zero_end = (str[count] == '\0');
    throw message{ str, is_zero_end ? count : N };
  }

  template <std::size_t N>
  void error_msg(const char (& str)[N])
  {
    std::print(stderr, "{}\n", make_message(str));
  }

  void error_msg(std::string_view message, std::string const & before, std::string const & after)
  {
    std::print(stderr, "{} {} {}", before, message, after);
  }

}
