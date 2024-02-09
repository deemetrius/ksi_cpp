#pragma once

#include "value_cat.hpp"
#include "value_type.hpp"
#include "value_bool.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::system_types
  {
    // types
    value_type t_module { converter_string("$module") };
    value_type t_cat    { converter_string("$cat"   ) };
    value_type t_type   { converter_string("$type"  ) };
    value_type t_bool   { converter_string("$bool"  ) };
    value_type t_int    { converter_string("$int"   ) };
    value_type t_float  { converter_string("$float" ) };
    value_type t_text   { converter_string("$text"  ) };

    // categories
    value_cat c_null  { converter_string("_null"  ) };
  };

  template <typename Type_config> inline types<Type_config>::ptr_type
  types<Type_config>::value_cat::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_cat; }

  template <typename Type_config> inline types<Type_config>::ptr_type
  types<Type_config>::value_type::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_type; }

  template <typename Type_config> inline types<Type_config>::ptr_type
  types<Type_config>::value_bool::get_type(ptr_system_types sys_types) const
  { return &sys_types->t_bool; }


} // ns
