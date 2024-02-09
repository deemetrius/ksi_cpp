#pragma once

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::value_type
    : public types<Type_config>::bases::value_static
  {
    // props
    t_string name;

    // ctor
    value_type(t_string p_name) : name{ std::move(p_name) }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;
  };


} // ns
