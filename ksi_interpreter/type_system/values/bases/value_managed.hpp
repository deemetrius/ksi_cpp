#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::value_managed
    : public system<Type_settings>::bases::value
  {
    static void value_goodbye(ptr_value_managed managed_value_handle)
    {
      delete managed_value_handle;
    }
    using ptr_farewell_function = decltype(& value_goodbye);

    // props
    ptr_farewell_function fn_way_out{ & value_goodbye };

    void assign_to_cell(care::ptr_cell to_cell) override;

    bases::ptr_value_managed try_get_managed() override
    {
      return this;
    }

    virtual void was_acquired(care::ptr_cell by_cell) = 0;
    virtual void was_redeemed(care::ptr_cell by_cell) = 0;
    virtual care::value_status determine_status() const = 0;

    using typename value::fn_close_type;
    fn_close_type get_close_function() const override
    {
      return (& close_function_managed);
    }

  private:
    static void close_function_managed(ptr_value & value_handle, care::ptr_cell cell_handle);
  };


} // ns
