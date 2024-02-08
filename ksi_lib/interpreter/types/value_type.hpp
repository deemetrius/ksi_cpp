#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::value_type
    : public value
  {
    // props
    t_string name;

    // ctor
    value_type(t_string p_name) : name{ std::move(p_name) }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override
    {
      return &sys_types->t_bool;
    }
  };


} // ns
