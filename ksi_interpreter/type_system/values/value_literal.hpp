#pragma once

#include "bases/value_placed.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_literal
    : public system<Type_settings>::bases::value_placed
  {
    // props
    info::literal_type literal;

    // ctor
    value_literal(info::literal_type lit) : literal{ lit }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string get_class_name() const override;

    void assign_to_cell(care::ptr_cell to_cell) override;
  };


} // ns
