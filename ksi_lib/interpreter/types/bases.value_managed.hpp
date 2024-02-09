#pragma once

#include "bases.value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_managed
    : public types<Type_config>::bases::value
  {
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
    virtual bool is_still_sticked() const = 0;
  };


} // ns
