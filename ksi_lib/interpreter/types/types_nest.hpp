#pragma once

#include "config_default.hpp"

namespace ksi::interpreter {


  template <typename Type_config = type_config>
  struct types
    : public Type_config
  {
    using config = Type_config;
    using typename config::t_bool;
    using typename config::t_integer;
    using typename config::t_floating;
    using typename config::t_string;
    using config::converter_string;


    struct value;

    struct value_cat;
    struct value_type;
    struct value_bool;

    struct system_types;


    using ptr_value = value *;
    using ptr_type = value_type *;
    using ptr_system_types = system_types *;
  };


} // ns
