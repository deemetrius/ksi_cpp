#pragma once

#include "../values/value_bool.hpp"
#include "../values/bases/value_static.hpp"
#include "../values/bases/value_managed.hpp"
#include "holder_value.hpp"
#include "junction.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::care::cell
  {
    static void cell_goodbye(ptr_cell cell_handle)
    {
      delete cell_handle;
    }
    using ptr_farewell_function = decltype(& cell_goodbye);

    // props
    union {
      values::value_bool v_bool;
    };
    ptr_value value_handle{ nullptr };
    care::junction junction_point;
    ptr_farewell_function fn_way_out{ & cell_goodbye };

    // ctor
    cell(holder_value && keep_value)
      : value_handle{ keep_value.release() } // ! keep should not be empty; otherwise .release() will throw !
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
      bases::value::fn_close_type fn_close = value_handle->get_close_function();
      fn_close(std::exchange(value_handle, nullptr), this);
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

  template <typename Type_settings>
  inline void system<Type_settings>::bases::value_static::assign_to_cell(care::ptr_cell to_cell)
  {
    to_cell->value_handle = this;
  }

  template <typename Type_settings>
  inline void system<Type_settings>::bases::value_managed::assign_to_cell(care::ptr_cell to_cell)
  {
    to_cell->value_handle = this;
    this->was_acquired(to_cell);
  }

  template <typename Type_settings>
  inline void system<Type_settings>::values::value_bool::assign_to_cell(care::ptr_cell to_cell)
  {
    to_cell->value_handle = new(&to_cell->v_bool) value_bool{this->flag};
  }


  template <typename Type_settings>
  void system<Type_settings>::bases::value_managed::close_function_managed(ptr_value & value_handle, care::ptr_cell cell_handle)
  {
    typename care::holder_value keep{ std::exchange(value_handle, nullptr)->try_get_managed() };
    keep->was_redeemed(cell_handle);
    // note here: so we let to holder_value::destructor do its job
  }


} // ns
