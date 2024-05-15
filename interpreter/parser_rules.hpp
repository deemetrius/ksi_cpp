#pragma once

#include "parser_data.hpp"

namespace ksi::interpreter
{

  struct rules
  {
    #include "parser_classes.inl"

    struct variable : is_name
    {};

    struct set_module_name
    {
      static void perform(state & st, sys::string & result)
      {
        //
      }
    };

    struct module_name : keyword<'@'>
    {};

    struct scope_file : is_place<module_name, set_module_name>
    {};

    struct scope_module
    {};

  }; // nest

}
