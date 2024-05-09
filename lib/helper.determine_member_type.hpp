#pragma once

namespace ksi::lib::traits {


  template <typename T>
  struct determine_member_type
  {
    using result = void;
  };

  template <typename Class, typename Type>
  struct determine_member_type<Type Class::*>
  {
    using result = Type;
  };


} // ns
