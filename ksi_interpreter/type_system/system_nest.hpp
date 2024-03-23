#pragma once

#include "type_config_default.hpp"
#include "ksi_lib/dict.hpp"
#include "ksi_lib/table.hpp"
#include <memory>
#include <set>
#include <map>
#include <string>

namespace ksi::interpreter {


  template <typename Type_config = type_config_default>
  struct system
    : public Type_config
  {
    using count_type = std::intptr_t;
    using index_type = std::size_t;

    using config = Type_config;
    using typename config::t_bool;
    using typename config::t_integer;
    using typename config::t_floating;
    using typename config::t_string;
    using config::converter_string;

    using t_string_internal = std::string;


    struct space_configuration;
    struct runtime_info;

    struct VM;
    struct patch_vm;


    struct values
    {
      struct value_cat;
      struct value_type;
      struct value_bool;
      struct value_array;
    };

    struct bases
    {
      struct value; // base for all values

      struct value_placed; // they live inside cell's union
      struct value_managed; // these ... outside

      struct value_static; // managed by: space_data
      struct value_ref_counted; // base to: value_string
      struct value_pointed; // maintains references (especially circular ones)

      using ptr_value_managed = value_managed *;
      using ptr_value_pointed = value_pointed *;
    };


    struct care
    {
      enum class value_status {
        n_should_stay,
        n_ready_for_delete,
        n_requires_point_examination_refs_circular_only,
      };

      struct tag_root {};
      struct tag_none {};
      static constexpr tag_root is_root{};
      static constexpr tag_none is_transitive{};

      struct point; // point placed to: bases::value_pointed
      struct junction; // junction live in: care::cell

      struct slot; // slots for arrays/maps/structs
      struct cell; // cells are referenced via slots

      struct holder_value; // carry values to depart sometimes
      struct holder_cell; // .. similar for cell
      struct root_finder;

      using ptr_point = point *;
      using ptr_cell = cell *;

      using in_point_set = std::set<ptr_cell>; // rels by cells
      // note: several slots from one point may refer to same cell
      // (so count is used as map-value)
      using in_junction_map = std::map<ptr_point, count_type>; // refs from value_pointed via slots
    };


    struct info
    {
      using dict_type = ksi::lib::dict<t_string>;
      using literal_type = dict_type::value_type const *;
      using dict_ptr_type = std::shared_ptr<dict_type>;
      using token_type = std::size_t;

      struct literal_less
      {
        bool operator () (literal_type lt, literal_type rt) const
        {
          return (lt->rank < rt->rank);
        }
      };

      struct meta_info
      {
        literal_type  name;
        index_type    position;

        static constexpr index_type meta_info::* auto_increment{ & meta_info::position };
      };
      struct property_info
        : public meta_info
      {
        //type_restriction
      };

      using table_of_properties = ksi::lib::table<property_info, & property_info::name, literal_less>;

      struct module_configuration
        : public meta_info
      {
        //constants
        table_of_properties  property_names;
      };

      struct var_names;
      struct sequence;

      using ptr_property_info = property_info *; // const?
    };

    struct runtime
    {
      struct thread_space;
        struct space_data; struct system_types;
        struct stack_values;
        struct call_stack_info; struct sequence_space;

      using stack_frame = values::value_array;
      using stack_frame_holder = care::holder_value;

      using ptr_stack_frame = stack_frame *;
      using ptr_sequence = info::sequence *;
      using ptr_sequence_space = sequence_space *;
    };


    struct execution;
    //struct instructions;


    using ptr_value = bases::value *;
    using ptr_type = values::value_type *;
    using ptr_space_configuration = space_configuration *;
    using ptr_thread_space = runtime::thread_space *;
    using ptr_runtime_info = runtime_info *;
    using ptr_system_types = runtime::system_types *;
  };


} // ns
