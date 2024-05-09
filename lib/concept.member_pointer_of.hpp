#pragma once

  #include <type_traits>
  #include "helper.recognize_class.hpp"

namespace ksi::lib::concepts {


  template <typename T, typename Struct>
  concept member_pointer_of = std::is_member_pointer_v<T> &&
    std::is_base_of_v<typename lib::traits::recognize_class<T>::result, Struct>
  ;


}