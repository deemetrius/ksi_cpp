#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::value_static
    : public system<Type_settings>::bases::value
  {
    void assign_to_cell(care::ptr_cell to_cell) override;

    using typename value::fn_close_type;
    fn_close_type get_close_function() const override
    {
      return (& close_function_static);
    }

  private:
    static void close_function_static(ptr_value & value_handle, care::ptr_cell cell_handle) { value_handle = nullptr; }

    /*
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

    care::status_of_value determine_status() const override
    {
      return care::status_of_value::n_should_stay;
    }
    */
  };


} // ns
