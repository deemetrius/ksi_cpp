#pragma once

#include "types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value
  {
    virtual ~value() = default;

    virtual ptr_type get_type(ptr_system_types sys_types) const = 0;

    virtual void was_acquired(care::ptr_cell by_cell) = 0;
    virtual void was_redeemed(care::ptr_cell by_cell) = 0;
    virtual bool is_still_sticked() const = 0;
  };


} // ns
