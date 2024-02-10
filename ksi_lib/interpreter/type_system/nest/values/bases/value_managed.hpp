#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_managed
    : public types<Type_config>::bases::value
  {
    static void value_goodbye(ptr_value_managed managed_value_handle)
    {
      delete managed_value_handle;
    }
    using ptr_farewell_function = decltype(& value_goodbye);

    // props
    ptr_farewell_function fn_way_out{ & value_goodbye };

    bool is_placed() const override
    {
      return false;
    }

    bases::ptr_value_managed try_get_managed() override
    {
      return this;
    }

    void assign_to_cell(care::ptr_cell to_cell) override;

    virtual void was_acquired(care::ptr_cell by_cell) = 0;
    virtual void was_redeemed(care::ptr_cell by_cell) = 0;
    virtual care::value_status determine_status() const = 0;
  };


} // ns
