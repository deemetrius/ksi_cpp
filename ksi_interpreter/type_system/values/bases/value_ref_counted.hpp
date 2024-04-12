#pragma once

#include "value_managed.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::value_ref_counted
    : public system<Type_settings>::bases::value_managed
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

    care::status_of_value determine_status() const override
    {
      return (
        (ref_count > 0) ?
        care::status_of_value::n_should_stay :
        care::status_of_value::n_ready_for_delete
      );
    }
  };


} // ns
