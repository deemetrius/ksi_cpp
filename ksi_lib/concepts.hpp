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


  template <typename Type, typename ... Args>
  concept any_of = ( std::is_same_v<Type, Args> || ... );


  template <typename T>
  struct class_of_member
  {
    using type = void;
  };

  template <typename T, typename Class>
  struct class_of_member<T Class::*>
  {
    using type = Class;
    using member_type = T;
  };

  template <typename T>
  using class_of_member_t = class_of_member<T>::type;


  template <typename T, typename Struct>
  concept pointer_to_member = std::is_member_pointer_v<T> && std::is_base_of_v<class_of_member_t<T>, Struct>;


} // end ns
