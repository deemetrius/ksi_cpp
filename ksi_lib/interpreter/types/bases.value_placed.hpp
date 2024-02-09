#pragma once

#include "bases.value.hpp"

namespace ksi::interpreter::errors {


  struct value_not_managed {};


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
      throw errors::value_not_managed{};
    }
  };


} // ns
