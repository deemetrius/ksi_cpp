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


} // ns
