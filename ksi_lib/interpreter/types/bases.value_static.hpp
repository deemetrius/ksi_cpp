#pragma once

#include "bases.value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_static
    : public types<Type_config>::bases::value
  {
    // props
    count_type ref_count{ 1 };

    // actions

    void was_acquired(care::ptr_cell by_cell) override
    {
      ++ref_count;
    }

    void was_redeemed(care::ptr_cell by_cell) override
    {
      --ref_count;
    }

    bool is_still_sticked() const override
    {
      return true;
    }
  };


} // ns
