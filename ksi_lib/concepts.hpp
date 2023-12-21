#pragma once

#include <type_traits>

namespace ksi::lib::concepts {

  template <typename Type>
  concept integer_type = std::is_integral_v<Type>;

  template <typename Type>
  concept integer_signed_type = std::is_signed_v<Type>;

  template <typename Type>
  concept integer_unsigned_type = std::is_unsigned_v<Type>;

} // end ns
