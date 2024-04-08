#pragma once

  #include "ksi_lib/type_actions/when.hpp"

namespace ksi::chars {


  namespace char_info {
    enum symbols : char {
      space   = ' ',

      tab     = '\t',
      cr      = '\r',
      lf      = '\n',
    };
  } // ns

  namespace wide_char_info {
    enum symbols : wchar_t {
      space   = L' ',

      tab     = L'\t',
      cr      = L'\r',
      lf      = L'\n',
    };
  } // ns

  template <typename Char>
  using symbols = type_traits::when<
    Char,
    type_traits::case_is< char     , char_info::symbols >,
    type_traits::case_is< wchar_t  , wide_char_info::symbols >
  >;


} // ns
