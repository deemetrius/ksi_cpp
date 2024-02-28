#pragma once

#include "value_managed.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::bases::value_static
    : public system<Type_config>::bases::value_managed
  {
    // props
    count_type use_count{ 1 };

    // actions

    void was_acquired(care::ptr_cell by_cell) override
    {
      ++use_count;
    }

    void was_redeemed(care::ptr_cell by_cell) override
    {
      --use_count;
    }

    care::value_status determine_status() const override
    {
      return care::value_status::n_should_stay;
    }
  };


} // ns
