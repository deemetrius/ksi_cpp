#pragma once

  #include <type_traits>

namespace ksi::type_traits {


  template <bool Condition, typename Result>
  struct option;


  namespace detail {
    template <typename ... Options>
    struct select_helper;

    template <bool Condition, typename Last_Result>
    struct select_helper< option<Condition, Last_Result> >
    {
      using type = std::conditional_t<
        Condition,
        Last_Result,
        void
      >;
    };

    template <bool Condition, typename First_Result, typename ... Rest_Options>
    struct select_helper< option<Condition, First_Result>, Rest_Options ... >
    {
      using type = std::conditional_t<
        Condition,
        First_Result,
        typename select_helper<Rest_Options ...>::type;
      >;
    };
  } // ns

  template <typename ... Options>
  using select = typename detail::select_helper<Options ...>::type;


} // ns
