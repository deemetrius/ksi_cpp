#pragma once

#include "config_default.hpp"
#include <set>
#include <map>

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
      struct value; // base for all values

      struct value_placed; // they live inside cell's union
      struct value_managed; // these ... outside

      struct value_static; // managed by: space_data
      struct value_ref_counted; // base to: value_string
      struct value_pointed; // maintains references (especially circular ones)

      using ptr_value_managed = value_managed *;
    };

    using ptr_value = bases::value *;

    struct care
    {
      enum class value_status {
        n_should_stay,
        n_ready_for_delete,
        n_holded_by_only_circular_refs,
        n_undetermined,
      };

      struct point; // point placed to: bases::value_pointed
      struct junction; // junction live in: care::cell

      struct slot; // slots for arrays/maps/structs
      struct cell; // cells are referenced via slots

      struct holder_value; // carry values to depart sometimes
      struct root_finder;

      using ptr_point = point *;
      using ptr_cell = cell *;

      using in_point_set = std::set<ptr_cell>; // rels by cells
      using in_junction_map = std::map<ptr_point, count_type>; // refs from value_pointed via slots
      // note: several slots from one point may refer to same cell
      // (so count is used as map-value)
    };
  };


} // ns
