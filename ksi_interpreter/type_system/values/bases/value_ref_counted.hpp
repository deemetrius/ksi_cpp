#pragma once

#include "value_managed.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::bases::value_ref_counted
    : public system<Type_config>::bases::value_managed
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

    care::value_status determine_status() const override
    {
      return (
        (ref_count > 0) ?
        care::value_status::n_should_stay :
        care::value_status::n_ready_for_delete
      );
    }
  };


} // ns
