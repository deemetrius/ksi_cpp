#pragma once

#include "../values/value_module.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::configuration
  {
    using table_of_modules = ksi::lib::table<
      typename values::value_module,
      & values::value_module::name,
      typename info::literal_less
    >;
  };


} // ns
