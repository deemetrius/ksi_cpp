#pragma once

#include "concepts.hpp"
#include <limits>
#include <algorithm>

namespace ksi::lib::conv {

  struct from_integer
  {
    template <typename Result>
    struct to;
  };


  template <concepts::integer_signed_type Result>
  struct from_integer::to<Result>
  {
    template <concepts::integer_signed_type Param>
    constexpr Result operator () (Param p) const
    {
      using limits_result = std::numeric_limits<Result>;
      using limits_param = std::numeric_limits<Param>;

      static_assert(limits_result::radix == limits_param::radix);

      if constexpr( limits_result::digits >= limits_param::digits )
      {
        return p;
      } else {
        Param min{ std::numeric_limits<Result>::min() };
        Param max{ std::numeric_limits<Result>::max() };
        return static_cast<Result>( std::clamp<Param>(p, min, max) );
      }
    }

    template <concepts::integer_unsigned_type Param>
    constexpr Result operator () (Param p) const
    {
      using limits_result = std::numeric_limits<Result>;
      using limits_param = std::numeric_limits<Param>;

      static_assert(limits_result::radix == limits_param::radix);

      if constexpr( limits_result::digits >= limits_param::digits )
      {
        return static_cast<Result>(p);
      } else {
        Param max{ std::numeric_limits<Result>::max() };
        return static_cast<Result>( std::min<Param>(p, max) );
      }
    }
  };


  template <concepts::integer_unsigned_type Result>
  struct from_integer::to<Result>
  {
    template <concepts::integer_unsigned_type Param>
    constexpr Result operator () (Param p) const
    {
      using limits_result = std::numeric_limits<Result>;
      using limits_param = std::numeric_limits<Param>;

      static_assert(limits_result::radix == limits_param::radix);

      if constexpr( limits_result::digits >= limits_param::digits )
      {
        return p;
      } else {
        Param max{ std::numeric_limits<Result>::max() };
        return static_cast<Result>( std::min<Param>(p, max) );
      }
    }

    template <concepts::integer_signed_type Param>
    constexpr Result operator () (Param p) const
    {
      using limits_result = std::numeric_limits<Result>;
      using limits_param = std::numeric_limits<Param>;

      static_assert(limits_result::radix == limits_param::radix);

      if constexpr( limits_result::digits >= limits_param::digits )
      {
        return static_cast<Result>( std::max<Param>(p, 0) );
      } else {
        Param max{
          static_cast<Param>( std::numeric_limits<Result>::max() )
        };
        return static_cast<Result>( std::clamp<Param>(p, 0, max) );
      }
    }
  };

} // end ns
