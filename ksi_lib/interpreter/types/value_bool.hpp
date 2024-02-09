#pragma once

#include "bases.value_ref_counted.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::value_bool
    : public types<Type_config>::bases::value_ref_counted
  {
    // props
    t_bool flag;

    // ctor
    value_bool(t_bool v) : flag{ v }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override
    {
      return &sys_types->t_bool;
    }
  };


} // ns
