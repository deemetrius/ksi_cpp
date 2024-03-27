#pragma once

#include "bases/is_hint.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_settings>
  struct system<Type_settings>::values::value_type
    : public system<Type_settings>::bases::is_hint
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // ctor
    value_type(info::meta_info params)
      : self_meta{ std::move(params) }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_type"s;
    }

    // is_hint

    bool match_type(ptr_type type) override
    {
      return (this == type); // is_base_of ~ if inheritance | or embed_properties
    }
  };


} // ns
