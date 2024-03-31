#pragma once

  #include "../values/value_module.hpp"
  #include "../values/value_cat.hpp"
  #include "../values/value_type.hpp"
  #include "../values/value_bool.hpp"
  #include "../values/value_array.hpp"

  #include <span>
  #include <iterator>
  #include <initializer_list>
  #include <vector>
  #include <cstring>

  #include "ksi_log/chars.hpp"

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

    struct helper_relationships
    {
      using cat_crew = std::initializer_list<ptr_cat>;
      using type_pack = std::initializer_list<ptr_type>;

      using names_type = std::vector<t_string_view>;
      using size_type = typename t_string_view::size_type;

      static size_type copy_chars(t_char * dst, t_string_view src)
      {
        size_type delta = src.size();
        std::memcpy(dst, src.data(), delta);
        return delta;
      }

      static t_string implode(t_string prefix, names_type elements, t_string separator)
      {
        if( elements.empty() ) { return prefix; }

        size_type count{ prefix.size() + separator.size() * (elements.size() - 1) };
        for( t_string_view it : elements )
        {
          count += it.size();
        }

        t_string ret(count, chars::symbols<t_char>::space);
        t_char * dst{ &(ret[0]) };

        dst += copy_chars(dst, prefix);

        {
          dst += copy_chars(dst, elements.front());
          std::span<t_string_view> span{ std::ranges::next( elements.begin() ), elements.end() };
          for( t_string_view it : span )
          {
            dst += copy_chars(dst, separator);
            dst += copy_chars(dst, it);
          }
        }

        return ret;
      }

      // props
      //? log_handle;
      names_type  circular, repeated;

      void sub_cats(ptr_cat parent, cat_crew cats)
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
          // todo: log notice
          t_string message = implode( converter_string(""), std::move(repeated), converter_string(", ") );
        }

        if( circular.size() > 0 )
        {
          // todo: log error
          t_string message = implode( converter_string(""), std::move(circular), converter_string(", ") );
        }
      }

      static void cat_belongs(ptr_cat cat, type_pack types)
      {
        using result_type = typename info::cat_includes::result_add;

        for( ptr_type each : types )
        {
          if( each->categories.add(cat) == result_type::was_already_included )
          {
            // todo: log notice
          }
        }
      }
    }; // struct helper

    // ctor
    system_types(typename info::ptr_dict_type dict_handle)
      : reg{ dict_handle }
    {
      helper_relationships  helper{/* log_handle */};

      // todo: tree of categories
      helper.sub_cats( c_any, {c_null, c_hint, c_struct} );

      // todo: assign categories to types
      helper.cat_belongs( c_hint, {t_cat, t_type} );
    }

    // props
    registrator  reg;

    // categories
    ptr_cat c_any     = reg.add_cat( converter_string("_any"    ) );
    ptr_cat c_null    = reg.add_cat( converter_string("_null"   ) );
    ptr_cat c_hint    = reg.add_cat( converter_string("_hint"   ) );
    ptr_cat c_struct  = reg.add_cat( converter_string("_struct" ) );

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

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::
    value_module::get_type(ptr_system_types sys_types) const
  { return sys_types->t_module; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::
    value_cat::get_type(ptr_system_types sys_types) const
  { return sys_types->t_cat; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::
    value_type::get_type(ptr_system_types sys_types) const
  { return sys_types->t_type; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::
    value_bool::get_type(ptr_system_types sys_types) const
  { return sys_types->t_bool; }

  template <typename Type_settings>
  inline system<Type_settings>::ptr_type
    system<Type_settings>::values::
    value_array::get_type(ptr_system_types sys_types) const
  { return sys_types->t_array; }


} // ns
