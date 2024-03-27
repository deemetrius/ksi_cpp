#pragma once

#include "type_settings_default.hpp"
#include "ksi_lib/dict.hpp"
#include "ksi_lib/table.hpp"
#include <set>
#include <map>
#include <string>

namespace ksi::interpreter {


  template <typename Type_settings = type_settings_default>
  struct system
    : public Type_settings
  {
    using count_type = std::intptr_t;
    using index_type = std::size_t;

    using type_settings = Type_settings;
    using typename type_settings::t_bool;
    using typename type_settings::t_integer;
    using typename type_settings::t_floating;
    using typename type_settings::t_string;
    using type_settings::converter_string;

    using t_string_internal = std::string;


    struct space_configuration;
    struct runtime_info;

    struct VM;
    struct patch_vm;


    struct values
    {
      struct value_module;
      struct value_cat;
      struct value_type;
      struct value_bool;
      struct value_array;

      struct system_types;
    };

    struct bases
    {
      struct value; // base for all values

      struct value_placed; // they live inside cell's union
      struct value_static; // they part of space_configuration

      struct value_managed;
      struct value_ref_counted; // base to: value_string
      struct value_pointed; // maintains references (especially circular ones)

      struct is_hint;

      using ptr_value_managed = value_managed *;
      using ptr_value_pointed = value_pointed *;
    };

    using ptr_value = bases::value *;
    using ptr_cat = values::value_cat *;
    using ptr_type = values::value_type *;
    using ptr_restricion = bases::is_hint *;


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
      using dict_result_add = dict_type::result_add;
      using literal_type = dict_type::value_type const *;
      using dict_holder_type = std::shared_ptr<dict_type>;
      using ptr_dict_type = dict_type *;
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
        index_type    position; // auto_increment ~ should be the first member for table
        literal_type  name;

        static constexpr index_type meta_info::*
          auto_increment{ & meta_info::position }
        ;
      };

      struct property_info
        : public meta_info
      {
        bool            is_readonly;
        ptr_restricion  type_restriction;
      };

      using table_of_properties = ksi::lib::table<property_info, & property_info::name, literal_less>;

      // todo: decide
      struct var_names;
      struct sequence;

      using ptr_property_info = property_info *; // maybe: ptr to const
    };

    struct configuration;

    struct runtime
    {
      struct thread_space;
        struct space_data;
        struct stack_values;
        struct call_stack_info; struct sequence_space;

      using stack_frame = values::value_array;
      using stack_frame_holder = care::holder_value;

      using ptr_stack_frame = stack_frame *;
      using ptr_sequence = info::sequence *;
      using ptr_sequence_space = sequence_space *;
    };


    struct execution;
    //todo: struct instructions;


    using ptr_space_configuration = space_configuration *;
    using ptr_thread_space = runtime::thread_space *;
    using ptr_runtime_info = runtime_info *;
    using ptr_system_types = values::system_types *;
  };


} // ns
