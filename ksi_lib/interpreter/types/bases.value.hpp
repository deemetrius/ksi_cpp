#pragma once

#include "types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value
  {
    virtual ~value() = default;

    virtual ptr_type get_type(ptr_system_types sys_types) const = 0;

    virtual bool is_placed() const = 0;
    virtual void assign_to_cell(care::ptr_cell to_cell) = 0;

    virtual bases::ptr_value_managed try_get_managed() = 0;
  };


} // ns