#pragma once

#include "bases/value_placed.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_index
    : public system<Type_settings>::bases::value_placed
  {
    // props
    index_type  n;

    value_index(index_type position) : n{ position }
    {}

    // interface

    void assign_to_cell(care::ptr_cell to_cell) override;

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string get_class_name() const override;
  };


} // ns
