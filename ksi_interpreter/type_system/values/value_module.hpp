#pragma once

/*
  constant:
    name
    value

  module_data // non-const values (per thread)
*/

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_module
    : public system<Type_settings>::bases::value_static
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // props
    //constants
    info::table_of_properties  property_names;

    // ctor
    value_module(info::meta_info params)
      : self_meta{ params }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_module"s;
    }
  };


} // ns
