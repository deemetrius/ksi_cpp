#pragma once

#include <cctype>
#include "parser_data.hpp"

namespace ksi::interpreter
{

  struct rules
  {
    #include "parser_classes.inl"


    struct colon : is_char<':'>
    {
      static const inline sys::string name = "colon"s;
    };

    struct variable : is_name
    {
      static const inline sys::string name = "variable"s;
    };

    template <typename Action>
    struct expression : is_place<digits, Action, is_last_rule>
    {
      static const inline sys::string name = "expression"s;
    };


    struct actions
    {

      struct set_number
      {
        static void perform(loader::state & st, sys::string & result)
        {
          execution::data_type data;
          data.get<sys::integer>() = std::atoll( result.c_str() );
        }
      };

      struct module_set_name
      {
        static void perform(loader::state & st, sys::string & result)
        {
          st.add_module(result);
          st.fn = &scope_module::function;
        }
      };

      struct module_set_cell_name
      {
        static void perform(loader::state & st, sys::string & result)
        {
          st.prepare.next_cell();

          st.prepare.name = std::move(result);

          st.fn = &after_module_cell_name::function;
        }
      };

      struct module_set_integral_number
      {
        static void perform(loader::state & st, sys::string & result)
        {
          st.prepare.seq_current_group().list_instructions.emplace_back<execution::instruction>(
            {
              instructions::literal_to_stack<sys::integer>,
              { .integer = std::atoll( result.c_str() ) }
            }
          );

          st.fn = &scope_module::function;
        }
      };

      struct module_start_constant_initialization
      {
        static void perform(loader::state & st, sys::string & result)
        {
          st.prepare.was_cell = loader::prepare_type::prev_cell_type::constant;
          st.fn = &expression<module_set_integral_number>::function;
        }
      };

    }; // actions


    struct module_name : keyword<'@'>
    {
      static const inline sys::string name = "module_name"s;
    };


    struct after_module_cell_name : is_place<colon, actions::module_start_constant_initialization>
    {};


    struct scope_module : is_place<variable, actions::module_set_cell_name, is_last_rule>
    {};


    struct scope_file : is_place<module_name, actions::module_set_name, is_last_rule>
    {};

  }; // nest

}
