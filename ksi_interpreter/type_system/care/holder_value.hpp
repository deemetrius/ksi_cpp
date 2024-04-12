#pragma once

#include "../values/bases/value_pointed.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::care::holder_value
  {
    using pointer = bases::ptr_value_managed;

    static void depart_value(pointer & handle)
    {
      typename bases::value_managed::ptr_farewell_function fn{ handle->fn_way_out };
      fn( std::exchange(handle, nullptr) );
    }

    // props
    pointer value_handle;

    // ctor: from value pointer
    holder_value(pointer pv) : value_handle{ pv }
    {}

    // ctor: move
    holder_value(holder_value && other) : value_handle{ other.release() }
    {}

    ~holder_value()
    {
      if( value_handle == nullptr ) { return; }
      switch( value_handle->determine_status() )
      {
        case care::status_of_value::n_requires_point_examination_refs_circular_only :
        try
        {
          typename bases::ptr_value_pointed value_pointed_handle = value_handle->try_get_pointed();
          care::root_finder finder;
          if( finder.is_point_rooted(& value_pointed_handle->point) ) { break; }
          // todo: collect | close slots
          depart_value(value_handle);
        }
        catch( std::bad_alloc const & e )
        {
          // todo: chain 'value_handle' as doubtful
          throw;
        }
        break;

        case care::status_of_value::n_ready_for_delete :
        depart_value(value_handle);
        break;

        default:;
      }
    }

    // actions

    pointer release()
    {
      if( value_handle == nullptr ) { throw errors::release_on_empty{"holder_value"} }
      return std::exchange(value_handle, nullptr);
    }

    pointer operator -> () const
    {
      return value_handle;
    }

    // ctors: no copy, no default
    holder_value(holder_value const &) = delete;
    holder_value() = delete;

    // assigns: no copy, no move
    holder_value & operator = (holder_value const &) = delete;
    holder_value & operator = (holder_value &&) = delete;
  };


} // ns
