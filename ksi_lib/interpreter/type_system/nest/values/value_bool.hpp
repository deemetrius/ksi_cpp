#pragma once

#include "bases/value_placed.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::value_bool
    : public types<Type_config>::bases::value_placed
  {
    // props
    t_bool flag;

    // ctor
    value_bool(t_bool v) : flag{ v }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    void assign_to_cell(care::ptr_cell to_cell) override;
  };


} // ns
