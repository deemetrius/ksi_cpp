#pragma once

#include "bases/value_placed.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_settings>
  struct system<Type_settings>::values::value_bool
    : public system<Type_settings>::bases::value_placed
  {
    // props
    t_bool flag;

    // ctor
    value_bool(t_bool v) : flag{ v }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_bool"s;
    }

    void assign_to_cell(care::ptr_cell to_cell) override;
  };


} // ns
