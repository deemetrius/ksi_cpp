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
    struct registrator
    {
      // props
      typename info::ptr_dict_type    dict_handle;
      configuration::table_of_types   all_types;
      configuration::table_of_cats    all_cats;

      // actions

      ptr_type add_type(t_string name)
      {
        typename info::dict_result_add res = dict_handle->add(name);
        return all_types.emplace_back( std::in_place_type<typename info::meta_info>, res.it->get_const() );
      }

      ptr_cat add_cat(t_string name)
      {
        typename info::dict_result_add res = dict_handle->add(name);
        return all_cats.emplace_back( std::in_place_type<typename info::meta_info>, res.it->get_const() );
      }
    };

    // ctor
    system_types(typename info::ptr_dict_type dict_handle)
      : reg{ dict_handle }
    {
      // todo: tree of categories
      c_null->cat_base_add(c_any);
    }

    // props
    registrator  reg;

    // categories
    ptr_cat c_any = reg.add_cat( converter_string("_any") );
    ptr_cat c_null = reg.add_cat( converter_string("_null") );

    // types
    ptr_type t_module = reg.add_type( converter_string("$module") );
    ptr_type t_cat    = reg.add_type( converter_string("$cat"   ) );
    ptr_type t_type   = reg.add_type( converter_string("$type"  ) );
    ptr_type t_bool   = reg.add_type( converter_string("$bool"  ) );
    //ptr_type t_int    = reg.add_type( converter_string("$int"   ) );
    //ptr_type t_float  = reg.add_type( converter_string("$float" ) );
    //ptr_type t_text   = reg.add_type( converter_string("$text"  ) );
    ptr_type t_array  = reg.add_type( converter_string("$array" ) );
  };

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_module::get_type(ptr_system_types sys_types) const
  { return sys_types->t_module; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_cat::get_type(ptr_system_types sys_types) const
  { return sys_types->t_cat; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_type::get_type(ptr_system_types sys_types) const
  { return sys_types->t_type; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_bool::get_type(ptr_system_types sys_types) const
  { return sys_types->t_bool; }

  template <typename Type_settings> inline system<Type_settings>::ptr_type
  system<Type_settings>::values::value_array::get_type(ptr_system_types sys_types) const
  { return sys_types->t_array; }


} // ns
