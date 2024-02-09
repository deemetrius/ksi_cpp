#pragma once

#include "../values/value_bool.hpp"
#include "holder_value.hpp"
#include <map>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::cell
  {
    using points_type = std::map<ptr_point, count_type>;

    // props
    union {
      value_bool v_bool;
    };
    ptr_value value_handle{ nullptr };
    points_type from_points;

    // ctor
    cell(holder_value && keep) : value_handle{ keep.release() }
    {
      value_handle->was_acquired(this);
    }

    ~cell()
    {
      close();
    }

    // actions

    bool is_free() const
    {
      return (
        (value_handle == nullptr) || value_handle->is_placed()
      );
    }

    void close()
    {
      if( is_free() ) { return; }
      holder_value keep{ std::exchange(value_handle, nullptr)->try_get_managed() };
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

  template <typename Type_config>
  inline void types<Type_config>::value_bool::assign_to_cell(care::ptr_cell to_cell)
  {
    to_cell->value_handle = new(&to_cell->v_bool) value_bool{this->flag};
  }

  template <typename Type_config>
  inline void types<Type_config>::bases::value_managed::assign_to_cell(care::ptr_cell to_cell)
  {
    to_cell->value_handle = this;
    this->was_acquired(to_cell);
  }


} // ns
