#pragma once

namespace ksi::lib {


  template <typename Type>
  struct walker
  {
    template <typename Range>
    static constexpr walker make_from_range(Range & r)
    {
      return { r.begin(), r.end() };
    }

    // props
    Type current;
    Type end;

    constexpr bool is_end() const
    {
      return (current == end);
    }

    constexpr void advance()
    {
      ++current;
    }
  };


} // ns
