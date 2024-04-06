#pragma once

#include "value_cat.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_type
    : public system<Type_settings>::bases::is_hint
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // props
    info::cat_includes  categories;

    // ctor
    value_type(info::meta_info params)
      : self_meta{ std::move(params) }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string get_class_name() const override;

    // is_hint

    bool match_type(ptr_type type_handle) override
    {
      return (this == type_handle); // is_base_of ~ if inheritance | or embed_properties
    }
  };


  template <typename Type_settings>
  inline bool system<Type_settings>::values::value_cat::match_type(ptr_type type_handle)
  {
    return type_handle->categories.has(this);
  }


} // ns
