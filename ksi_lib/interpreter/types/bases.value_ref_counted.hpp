#pragma once

#include "bases.value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_ref_counted
    : public types<Type_config>::bases::value
  {
    // props
    count_type ref_count{ 0 };

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
      return (ref_count > 0);
    }
  };


} // ns
