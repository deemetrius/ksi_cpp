#pragma once

#include "value.hpp"
#include "care.holder_value.hpp"
#include <map>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::cell
  {
    using ptr_value = types<Type_config>::ptr_value;
    using points_type = std::map<ptr_point, count_type>;

    // props
    ptr_value value_handle;
    points_type from_points;

    // ctor
    cell(holder_value && keep) : value_handle{ keep.release()) }
    {
      value_handle->was_acquired(this);
    }

    ~cell()
    {
      close();
    }

    // actions

    bool empty() const
    {
      return (value_handle == nullptr);
    }

    void close()
    {
      if( empty() ) { return; }
      holder_value keep{ std::exchange(value_handle, nullptr) };
      keep->was_redeemed(this);
    }

    // ctors: no copy, no move, no default
    cell(cell const &) = delete;
    cell(cell &&) = delete;
    cell() = delete;

    // assigns: no copy, no move
    cell & operator = (cell const &) = delete;
    cell & operator = (cell &&) = delete;
  };


} // ns
