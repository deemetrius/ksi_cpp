#pragma once

#include "value.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::bases::value_placed
    : public system<Type_config>::bases::value
  {
    using typename value::fn_close_type;
    fn_close_type get_close_function() const override
    {
      return (& close_function_placed);
    }

  private:
    static void close_function_placed(ptr_value & value_handle, care::ptr_cell cell_handle)
    {
      if constexpr( config::call_destructor_for_simple_placed_values )
      {
        value_handle->~value();
      }
      value_handle = nullptr;
    }
  };


} // ns
