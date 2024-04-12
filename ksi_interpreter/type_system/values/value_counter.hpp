#pragma once

#include "bases/value_placed.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_counter
    : public system<Type_settings>::bases::value_placed
  {
    // props
    index_type  value;

    value_counter(index_type v) : value{ v }
    {}

    // interface

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string get_class_name() const override;

    void assign_to_cell(care::ptr_cell to_cell) override;
  };


} // ns
