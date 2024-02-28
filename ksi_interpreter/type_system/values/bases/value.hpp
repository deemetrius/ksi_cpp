#pragma once

#include "../../system_nest.hpp"
#include "ksi_interpreter/interpreter_errors.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_config>
  struct system<Type_config>::bases::value
  {
    virtual ~value() = default;

    virtual ptr_type get_type(ptr_system_types sys_types) const = 0;
    virtual t_string_internal get_class_name() const = 0;

    virtual bool is_placed() const = 0;
    virtual void assign_to_cell(care::ptr_cell to_cell) = 0;

    virtual bases::ptr_value_managed try_get_managed()
    {
      throw errors::method_not_supported{ get_class_name() + "::try_get_managed()"s };
    }

    virtual bases::ptr_value_pointed try_get_pointed()
    {
      throw errors::method_not_supported{ get_class_name() + "::try_get_pointed()"s };
    }
  };


} // ns
