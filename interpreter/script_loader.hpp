#pragma once

#include "vm_config.hpp"
#include <cctype>
#include "lib/dict_diff.hpp"

namespace ksi::interpreter::loader
{

  struct file_position
  {
    sys::string::const_iterator  current, last;

    bool is_end() const { return (current == last); }
  };

}
namespace ksi::interpreter::loader
{

  struct parser_data
  {
    struct group_action_information
    {
      using function_type = bool (*) (loader::file_position & pos, parser_data * data);

      function_type current_function;
      sys::string   cell_name;
      sys::integer  number{};
    };

    vm_config                 * target_configuration;
    group_action_information  info;

    lib::dict_diff<sys::string>                     dict{ target_configuration->dict };
    sys::static_table<configuration::module_config> modules;
    configuration::module_config                    * current_module{ nullptr };
  };

  template <typename Rule, typename NextRule, typename = void>
  struct rule
  {
    static bool parser_function(loader::file_position & pos, parser_data * data)
    {
      if( Rule{}.recognize(pos) )
      {
        data->info.current_function = &NextRule::parser_function;
        return true;
      }

      return false;
    }
  };

  template <typename Rule, typename NextRule>
  struct rule<Rule, NextRule, std::void_t<decltype( std::declval<Rule>().action_fn )>>
  {
    static bool parser_function(loader::file_position & pos, parser_data * data)
    {
      Rule r;

      if( r.recognize(pos) ) { r.action_fn(r.result, data); }
      else{ return false; }

      data->info.current_function = &NextRule::parser_function;
      return true;
    }
  };

}
namespace ksi::interpreter::loader::rules
{
  /*
  // before module's variable type restriction can be not specified (directly in text) it can be deduced from sequence initializer

  // before local variables it should be specified type restriction
  */
  struct actions
  {
    static bool add_constant_to_module(parser_data * data)
    {
      sys::literal name = data->dict.add( std::move(data->info.cell_name) );
      try
      {
        data->current_module->constants.append_row<configuration::module_config::read_only_cell>();
      }
      catch( lib::table_key_not_unique const & )
      {
        error_msg("module already have such cell name"sv, data->current_module->name->first, data->info.cell_name);
        return false;
      }

      return true;
    }

    static void add_module(sys::string & name, parser_data * data)
    {
      sys::literal module_name = data->dict.add( std::move(name) );
      data->current_module = data->modules.append_row<type_system::meta::meta_information>(module_name);
    }

    static void set_cell_name(sys::string & name, parser_data * out)
    {
      out->info.cell_name = std::move(name);
    }

    static void set_number(sys::string & digits, parser_data * out)
    {
      out->info.number = inner::number_from_string(digits);
    }

    struct inner
    {
      static sys::integer number_from_string(sys::string const & str)
      {
        return std::atoll(str.c_str());
      }
    };
  };

  template <sys::character C>
  struct is_char
  {
    bool recognize(loader::file_position & pos)
    {
      if( *pos.current == C )
      {
        ++pos.current;
        return true;
      }

      return false;
    }
  };

  struct colon : is_char<':'>
  {};

  using fn_type = decltype(&actions::add_module);

  struct number
  {
    sys::string result;

    bool recognize(loader::file_position & pos)
    {
      if( std::isdigit(*pos.current) )
      {
        sys::string::const_iterator first = pos.current;
        ++pos.current;

        while( std::isdigit(*pos.current) )
        {
          ++pos.current;
        }

        result.assign(first, pos.current);

        return true;
      }

      return false;
    }

    fn_type action_fn = &actions::set_number;
  };

  struct is_name
  {
    static bool distinguish_name(loader::file_position & position, sys::string & result)
    {
      if( ! std::isalpha(*position.current) ) { return false; }
      ++position.current;

      while( std::isalnum(*position.current) )
      { ++position.current; }

      result.assign(position.current, position.last);

      return true;
    }
  };

  struct module_name : is_name
  {
    sys::string result;

    bool recognize(loader::file_position & pos)
    {
      if( *pos.current != '@' ) { return false; }
      loader::file_position new_pos = pos;
      ++new_pos.current;

      if( distinguish_name(new_pos, result) )
      {
        pos = new_pos;
        return true;
      }

      return false;
    }

    fn_type action_fn{ &actions::add_module };
  };

  struct variable_name : is_name
  {
    sys::string result;

    bool recognize(loader::file_position & pos)
    {
      return distinguish_name(pos, result);
    }

    fn_type action_fn{ &actions::set_cell_name };
  };

  struct nothing
  {
    bool recognize(loader::file_position & pos) { return true; }
  };

  struct module_add_constant
  {
    static bool parser_function(loader::file_position & pos, parser_data * data);
  };

  struct module_start_constant_initialization : rule<number, module_add_constant>
  {
  };

  struct after_module_cell_name : rule<colon, module_start_constant_initialization>
  {};

  struct inside_module : rule<variable_name, after_module_cell_name>
  {
  };

  struct inside_file : rule<module_name, inside_module>
  {};


  inline bool module_add_constant::parser_function(loader::file_position & pos, parser_data * data)
  {
    data->info.current_function = &inside_module::parser_function;
    return actions::add_constant_to_module(data);
  }

}
namespace ksi::interpreter::loader
{

  struct script_parser : parser_data
  {
    script_parser(vm_config * vm_configuration) : parser_data{ vm_configuration, &rules::inside_file::parser_function } {}

    static bool is_space(sys::character ch) { return (ch == ' ' || ch == '\n'); }

    static void skip_spaces(file_position & pos)
    {
      while( is_space( *pos.current ) )
      {
        ++pos.current;
        if( pos.is_end() ) { return; }
      }
    }

    void parse(sys::string const & file_content)
    {
      file_position pos{ file_content.cbegin(), file_content.cend() };
      if( pos.is_end() ) { return; }

      skip_spaces(pos);

      while( info.current_function(pos, this) );
    }
  };

  struct script_loader
  {
    script_parser parser;

    script_loader(vm_config * vm_configuration) : parser{ vm_configuration } {}
  };

}