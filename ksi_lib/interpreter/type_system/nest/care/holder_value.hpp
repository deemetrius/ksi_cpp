#pragma once

#include "../values/bases/value_managed.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::holder_value
  {
    using pointer = bases::ptr_value_managed;

    static void depart_value(pointer & handle)
    {
      typename base::value_managed::ptr_farewell_function fn{ handle->fn_way_out };
      fn( std::exchange(handle, nullptr) );
    }

    // props
    pointer value_handle;

    // ctor: from value pointer
    holder_value(pointer pv) : value_handle{ pv }
    {}

    // ctor: move
    holder_value(holder_value && other) : value_handle{ other.release()) }
    {}

    ~holder_value()
    {
      if( (value_handle == nullptr) || value_handle->is_still_sticked() ) { return; }
      // todo: collect
      depart_value(value_handle);
    }

    // actions

    pointer release()
    {
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
