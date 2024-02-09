#pragma once

#include "types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::holder_value
  {
    using ptr_value = types<Type_config>::ptr_value;

    // props
    ptr_value value_handle;

    // ctor: from value pointer
    holder_value(ptr_value pv) : value_handle{ pv }
    {}

    // ctor: move
    holder_value(holder_value && other) : value_handle{ other.release()) }
    {}

    ~holder()
    {
      if( (value_handle == nullptr) || value_handle->is_still_sticked() ) { return; }
      // todo: collect
    }

    // actions

    ptr_value release()
    {
      return std::exchange(value_handle, nullptr);
    }

    ptr_value operator -> () const
    {
      return value_handle;
    }

    // ctors: no copy, no default
    holder_value(holder const &) = delete;
    holder_value() = delete;

    // assigns: no copy, no move
    holder_value & operator = (holder_value const &) = delete;
    holder_value & operator = (holder_value &&) = delete;
  };


} // ns
