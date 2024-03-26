#pragma once

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_settings>
  struct system<Type_settings>::values::value_type
    : public system<Type_settings>::bases::value_static
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // ctor
    value_type(info::meta_info params)
      : self_meta{ params }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_type"s;
    }
  };


} // ns
