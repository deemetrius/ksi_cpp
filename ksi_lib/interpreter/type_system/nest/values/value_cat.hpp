#pragma once

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_config>
  struct types<Type_config>::values::value_cat
    : public types::bases::value_static
  {
    // props
    t_string name;

    // ctor
    value_cat(t_string p_name) : name{ std::move(p_name) }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_cat"s;
    }
  };


} // ns
