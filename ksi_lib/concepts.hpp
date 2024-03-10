#pragma once

#include <type_traits>

namespace ksi::concepts {


  template <typename Type>
  concept integer_type = std::is_integral_v<Type>;


  template <typename Type>
  concept integer_signed_type = std::is_signed_v<Type>;


  template <typename Type>
  concept integer_unsigned_type = std::is_unsigned_v<Type>;


  template <typename Type, typename Container>
  concept iterator_of =
    std::is_same_v<Type, typename Container::iterator> ||
    std::is_same_v<Type, typename Container::const_iterator> ||
    std::is_same_v<Type, typename Container::const_reverse_iterator> ||
    std::is_same_v<Type, typename Container::reverse_iterator>;


} // end ns
