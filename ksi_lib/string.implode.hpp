#pragma once

  #include <string>
  #include <initializer_list>
  #include <string_view>
  #include <vector>

  #include "ksi_log/chars.hpp"

  #include <span>

namespace ksi::lib {


  template <typename Container>
  auto sum_sizes(Container const & elements) -> typename Container::value_type::size_type
  {
    typename Container::value_type::size_type   ret{ 0 };

    for( auto const & each : elements )
    {
      ret += each.size();
    }

    return ret;
  }


  template <typename Char>
  typename std::basic_string_view<Char>::size_type
    copy_chars(Char * dst, std::basic_string_view<Char> src)
  {
    typename std::basic_string_view<Char>::size_type  delta{ src.size() };

    std::char_traits<Char>::copy(dst, src.data(), delta);

    return delta;
  }


  template <typename Char>
  std::basic_string<Char> implode(
    std::initializer_list< std::basic_string_view<Char> >   prefixes,
    std::basic_string<Char>                                 ending,
    std::vector< std::basic_string_view<Char> >             elements,
    std::basic_string<Char>                                 separator
  )
  {
    using t_string_view = std::basic_string_view<Char>;
    using size_type = typename t_string_view::size_type;

    // calc count
    size_type count{
      sum_sizes(prefixes) +
      sum_sizes(elements) +
      (elements.size() - 1) * separator.size() +
      ending.size()
    };

    // prepare result
    std::basic_string<Char>   result(count, chars::symbols<Char>::space);
    Char * dst = result.data();

    // add prefixes
    for( t_string_view each : prefixes )
    {
      dst += copy_chars(dst, each);
    }

    // add elements separated
    if( not elements.empty() )
    {
      dst += copy_chars( dst, elements.front() );

      std::span<t_string_view> span{ std::ranges::next( elements.begin() ), elements.end() };
      for( t_string_view each : span )
      {
        dst += copy_chars<Char>(dst, separator);
        dst += copy_chars(dst, each);
      }
    }

    dst += copy_chars<Char>( dst, ending );

    return result;
  }


} // ns
