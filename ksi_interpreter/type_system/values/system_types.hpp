#pragma once

#include "../values/value_module.hpp"
#include "../values/value_cat.hpp"
#include "../values/value_type.hpp"
#include "../values/value_bool.hpp"
#include "../values/value_array.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::system_types
  {
    // types
    values::value_type t_module { converter_string("$module") };
    values::value_type t_cat    { converter_string("$cat"   ) };
    values::value_type t_type   { converter_string("$type"  ) };
    values::value_type t_bool   { converter_string("$bool"  ) };
    values::value_type t_int    { converter_string("$int"   ) };
    values::value_type t_float  { converter_string("$float" ) };
    values::value_type t_text   { converter_string("$text"  ) };
    values::value_type t_array  { converter_string("$array" ) };

    // categories
    values::value_cat c_null  { converter_string("_null"  ) };
  };

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_module::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_module; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_cat::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_cat; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_type::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_type; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_bool::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_bool; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_array::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_array; }


} // ns
