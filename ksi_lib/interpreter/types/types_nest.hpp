#pragma once

#include "config_default.hpp"

namespace ksi::interpreter {


  template <typename Type_config = type_config>
  struct types
    : public Type_config
  {
    using count_type = std::intptr_t;

    using config = Type_config;
    using typename config::t_bool;
    using typename config::t_integer;
    using typename config::t_floating;
    using typename config::t_string;
    using config::converter_string;

    struct value_cat;
    struct value_type;
    struct value_bool;

    struct system_types;

    using ptr_type = value_type *;
    using ptr_system_types = system_types *;

    struct bases
    {
      struct value;
      struct value_static; // managed by: space_data
      struct value_ref_counted;
      struct value_pointed;
    };

    using ptr_value = bases::value *;

    struct care
    {
      struct point;
      struct slot;
      struct cell;

      struct holder_value;

      using ptr_point = point *;
      using ptr_cell = cell *;
    };
  };


} // ns
