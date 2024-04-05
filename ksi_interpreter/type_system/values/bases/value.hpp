#pragma once

#include "../../system_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::value
  {
    virtual ~value() = default;

  private:
    static void close_function_proto(ptr_value & value_handle, care::ptr_cell cell_handle);

  public:
    using fn_close_type = decltype( & close_function_proto );

    virtual fn_close_type get_close_function() const = 0;

    virtual ptr_type get_type(ptr_system_types sys_types) const = 0;
    virtual t_string get_class_name() const = 0;

    virtual void assign_to_cell(care::ptr_cell to_cell) = 0;

    virtual bases::ptr_value_managed try_get_managed()
    {
      throw typename errors::method_not_supported{ get_class_name() + converter_string("::try_get_managed()"sv) };
    }

    virtual bases::ptr_value_pointed try_get_pointed()
    {
      throw typename errors::method_not_supported{ get_class_name() + converter_string("::try_get_pointed()"sv) };
    }
  };


} // ns
