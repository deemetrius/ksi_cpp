#pragma once

#include "value.hpp"

namespace ksi::interpreter::errors {


  struct base { std::string msg; };
  struct internal : public base {};
  struct method_not_supported : public internal {};


}

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_placed
    : public types<Type_config>::bases::value
  {
    bool is_placed() const override
    {
      return true;
    }

    bases::ptr_value_managed try_get_managed() override
    {
      throw errors::method_not_supported{ "value_placed::try_get_managed()" };
    }
  };


} // ns
