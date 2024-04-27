#pragma once

namespace ksi::lib::traits {


  template <typename T>
  struct recognize_class
  {
    using result = void;
  };

  template <typename Class, typename Type>
  struct recognize_class<Type Class::*>
  {
    using result = Class;
  };


} // ns
