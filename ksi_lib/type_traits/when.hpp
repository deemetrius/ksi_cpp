#pragma once

  #include <type_traits>

namespace ksi::type_traits {


  struct no_result;


  template <typename Match, typename Result>
  struct case_is;


  namespace detail {
    template <typename Type, typename ... Cases>
    struct when_helper;

    template <typename Type, typename Match, typename Last_Result>
    struct when_helper< Type, case_is<Match, Last_Result> >
    {
      using type = std::conditional_t<
        std::is_same_v<Type, Match>,
        Last_Result,
        no_result
      >;
    };

    template <typename Type, typename Match, typename Result, typename ... Rest_Cases>
    struct when_helper< Type, case_is<Match, Result>, Rest_Cases ... >
    {
      using type = std::conditional_t<
        std::is_same_v<Type, Match>,
        Result,
        typename when_helper<Type, Rest_Cases ...>::type
      >;
    };
  } // ns

  template <typename Type, typename ... Cases>
  using when = typename detail::when_helper<Type, Cases ...>::type;


} // ns
