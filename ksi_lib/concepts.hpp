#pragma once

#include <type_traits>

namespace ksi::type_traits {


  template <typename T>
  struct helper_pointer_to_member
  {
    using source_type = T;
    using class_type = void;
    using member_type = void;
  };

  template <typename T, typename Class>
  struct helper_pointer_to_member<T Class::*>
  {
    using source_type = T Class::*;
    using class_type = Class;
    using member_type = T;
  };

  template <typename T>
  using pointer_to_member = helper_pointer_to_member< std::remove_cv_t<T> >;


} // ns

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


  template <typename Type, typename ... Args>
  concept any_of = ( std::is_same_v<Type, Args> || ... );


  template <typename T, typename Struct>
  concept pointer_to_member_of =
    std::is_member_pointer_v<T> &&
    std::is_base_of_v<typename ksi::type_traits::pointer_to_member<T>::class_type, Struct>
  ;


} // end ns
