#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::bases::value_placed
    : public system<Type_config>::bases::value
  {
    bool is_placed() const override
    {
      return true;
    }
  };


} // ns