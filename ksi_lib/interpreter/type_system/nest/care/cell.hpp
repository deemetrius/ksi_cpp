#pragma once

#include "../values/value_bool.hpp"
#include "junction.hpp"
#include "holder_value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::cell
  {
    // props
    union {
      value_bool v_bool;
    };
    ptr_value value_handle{ nullptr };
    care::junction junction_point;

    // ctor
    cell(holder_value && keep)
      : value_handle{ keep.release() } // ! keep should not be empty !
    {
      value_handle->was_acquired(this);
    }

    ~cell() { close(); }

    // actions

    ptr_value get_value() const
    {
      return value_handle;
    }

    bool empty() const
    {
      return (value_handle == nullptr);
    }

    void close()
    {
      if( empty() ) { return; }
      if( value_handle->is_placed() )
      {
        if constexpr( config::call_destructor_for_simple_placed_values )
        {
          value_handle->~value();
          value_handle = nullptr;
        }
        return;
      }
      holder_value keep{ std::exchange(value_handle, nullptr)->try_get_managed() };
      keep->was_redeemed(this);
      // note here: so we let to holder_value::destructor do its job
    }

    bool assign_from_cell(ptr_cell other_cell_handle)
    {
      if( other_cell_handle == this ) { return false; }
      if( other_cell_handle->empty() )
      {
        // todo: log
        return false;
      }
      this->close();
      other_cell_handle->get_value()->assign_to_cell(this);
      return true;
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
