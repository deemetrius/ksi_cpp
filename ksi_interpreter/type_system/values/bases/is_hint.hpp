#pragma once

#include "value_static.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::is_hint
    : public system<Type_settings>::bases::value_static
  {
    // actions

    virtual bool match_type(ptr_type type) = 0;
    bool match_value(ptr_value val)
    {
      return this->match_type( val->get_type() );
    }
  };


} // ns
