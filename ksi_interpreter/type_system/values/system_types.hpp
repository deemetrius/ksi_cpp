#pragma once

  #include "../values/value_module.hpp"
  #include "../values/value_cat.hpp"
  #include "../values/value_type.hpp"
  #include "../values/value_bool.hpp"
  #include "../values/value_array.hpp"

  #include "../nest/log.messages.hpp"

  #include "ksi_lib/string.implode.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::system_types
  {
    struct registrator
    {
      // props
      typename info::ptr_dict_type          dict_handle;
      typename log::internal_interface_ptr  log_handle;
      configuration::table_of_types         all_types;
      configuration::table_of_cats          all_cats;

      // actions

      ptr_type add_type(t_string name)
      {
        typename info::dict_result_add res = dict_handle->add(name);
        // todo: check if name is unique
        return all_types.emplace_back( std::in_place_type<typename info::meta_info>, res.it->get_const() );
      }

      ptr_cat add_cat(t_string name)
      {
        typename info::dict_result_add res = dict_handle->add(name);
        // todo: check if name is unique
        return all_cats.emplace_back( std::in_place_type<typename info::meta_info>, res.it->get_const() );
      }
    };

    struct helper_relationships
    {
      using cat_crew = std::initializer_list<ptr_cat>;
      using type_pack = std::initializer_list<ptr_type>;

      using names_type = std::vector<t_string_view>;
      using size_type = typename t_string_view::size_type;

      // props
      typename log::internal_interface_ptr  log_handle;
      names_type                            circular, repeated, repeated_types;

      void sub_cats(ptr_cat parent, cat_crew cats, std::source_location src_location = std::source_location::current() )
      {
        for( ptr_cat each : cats )
        {
          switch( each->cat_add_parent(parent) )
          {
            using result_type = typename values::value_cat::result_add_parent;

            case result_type::circular_cats_forbidden :
              circular.push_back(each->name->name);
            break;

            case result_type::same_twice :
              repeated.push_back(each->name->name);
            break;

            default: ;
          }
        } // for

        if( repeated.size() > 0 )
        {
          typename log::message msg{
            ksi::lib::implode<t_char>(
              // prefixes, ending, elements, separator
              {
                parent->name->name,
                converter_string(" ~ category set as parent again to following categories: "s)
              },
              {},
              std::move( this->repeated ), // this->repeated ~ vector empty after move
              converter_string(", "s)
            ),
            log::keys::cat_set_as_parent_again_for_cats
          };
          log_handle->add({&msg, src_location});
        }

        if( circular.size() > 0 )
        {
          typename log::message msg{
            ksi::lib::implode<t_char>(
              // prefixes, ending, elements, separator
              {
                parent->name->name,
                converter_string(" ~ category can't be parent for: "s)
              },
              converter_string(" (Cirular dependencies are not allowed)"s),
              std::move( this->circular ), // this->circular ~ vector empty after move
              converter_string(", "s)
            ),
            log::keys::cat_circular_dependencies
          };
          log_handle->add({&msg, src_location});
        }
      }

      void cat_belongs(ptr_cat cat, type_pack types, std::source_location src_location = std::source_location::current() )
      {
        using result_type = typename info::cat_includes::result_add;

        for( ptr_type each : types )
        {
          if( each->categories.add(cat) == result_type::was_already_included )
          {
            repeated_types.push_back(each->name->name);
          }
        }

        if( repeated_types.size() > 0 )
        {
          typename log::message msg{
            ksi::lib::implode<t_char>(
              // prefixes, ending, elements, separator
              {
                cat->name->name,
                converter_string(" ~ category was already assigned for these types: "s)
              },
              {},
              std::move( this->repeated_types ), // this->repeated_types ~ vector empty after move
              converter_string(", "s)
            ),
            log::keys::cat_already_assigned_for_types
          };
          log_handle->add({&msg, src_location});
        }
      }
    }; // struct helper

    // ctor
    system_types(typename info::ptr_dict_type dict_handle, typename log::internal_interface_ptr log_handle)
      : reg{ dict_handle, log_handle }
    {
      helper_relationships  helper{ log_handle };

      // todo: tree of categories
      helper.sub_cats( c_any, {c_null, c_hint, c_struct} );

      // todo: assign categories to types
      helper.cat_belongs( c_hint, {t_cat, t_type} );
      helper.cat_belongs( c_hint, {t_cat, t_type} );
    }

    // props
    registrator  reg;

    // categories
    ptr_cat c_any     = reg.add_cat( converter_string("_any"s     ) );
    ptr_cat c_null    = reg.add_cat( converter_string("_null"s    ) );
    ptr_cat c_hint    = reg.add_cat( converter_string("_hint"s    ) );
    ptr_cat c_struct  = reg.add_cat( converter_string("_struct"s  ) );

    // types
    ptr_type t_module = reg.add_type( converter_string("$module"s ) );
    ptr_type t_cat    = reg.add_type( converter_string("$cat"s    ) );
    ptr_type t_type   = reg.add_type( converter_string("$type"s   ) );
    ptr_type t_bool   = reg.add_type( converter_string("$bool"s   ) );
      //ptr_type t_int    = reg.add_type( converter_string("$int"s    ) );
      //ptr_type t_float  = reg.add_type( converter_string("$float"s  ) );
      //ptr_type t_text   = reg.add_type( converter_string("$text"s   ) );
    ptr_type t_array  = reg.add_type( converter_string("$array"s  ) );
  };


  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::value_module::   get_type(ptr_system_types sys_types) const
  { return sys_types->t_module; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::value_cat::      get_type(ptr_system_types sys_types) const
  { return sys_types->t_cat; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::value_type::     get_type(ptr_system_types sys_types) const
  { return sys_types->t_type; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::value_bool::     get_type(ptr_system_types sys_types) const
  { return sys_types->t_bool; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::value_array::    get_type(ptr_system_types sys_types) const
  { return sys_types->t_array; }


  template <typename Type_settings>
  inline system<Type_settings>::t_string    system<Type_settings>::values::value_module::   get_class_name() const
  { return converter_string("value_module"sv); }

  template <typename Type_settings>
  inline system<Type_settings>::t_string    system<Type_settings>::values::value_cat::      get_class_name() const
  { return converter_string("value_cat"sv); }

  template <typename Type_settings>
  inline system<Type_settings>::t_string    system<Type_settings>::values::value_type::     get_class_name() const
  { return converter_string("value_type"sv); }

  template <typename Type_settings>
  inline system<Type_settings>::t_string    system<Type_settings>::values::value_bool::     get_class_name() const
  { return converter_string("value_bool"sv); }

  template <typename Type_settings>
  inline system<Type_settings>::t_string    system<Type_settings>::values::value_array::    get_class_name() const
  { return converter_string("value_array"sv); }


} // ns
