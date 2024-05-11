#pragma once

#include "vm_config.hpp"
#include <cctype>
#include "lib/dict_diff.hpp"

#include "lib/static_text.hpp"

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
  using namespace std::string_literals;

  inline void skip_space(typename sys::string::const_iterator & here)
  {
    if( std::isspace(*here) )
    {
      ++here;
    }
  }

  struct parser_data
  {
    struct group_action_information
    {
      sys::string   cell_name;
      sys::integer  number{};
    };

    vm_config                 * target_configuration;
    group_action_information  info;

    lib::dict_diff<sys::string>                     dict{ target_configuration->dict };
    sys::static_table<configuration::module_config> modules;
    configuration::module_config                    * current_module{ nullptr };
  };

  struct state
  {
    using function_type = state (*) (loader::file_position & pos, parser_data * data);

    std::string       message;
    function_type     current_function;
  };

  template <typename Rule, typename NextRule, typename = void>
  struct rule
  {
    static state parser_function(loader::file_position & pos, parser_data * data)
    {
      if( Rule{}.recognize(pos) )
      {
        return NextRule::parser_function(pos, data);
      }

      return {};
    }
  };

  template <typename Rule, typename NextRule>
  struct rule<Rule, NextRule, std::void_t<decltype( std::declval<Rule>().action_fn )>>
  {
    static state parser_function(loader::file_position & pos, parser_data * data)
    {
      Rule r;

      if( r.recognize(pos) == false )
      {
        return { "Token don't recognized"s, nullptr };
      }

      return { r.action_fn(r.result, data), &NextRule::parser_function };
    }
  };

  template <typename Condition, typename Action>
  struct sentence
  {
    static state parser_function(loader::file_position & pos, parser_data * data)
    {
      Condition cond;

      if( cond.recognize(pos) )
      {
        return Action::fn(cond.result, data);
      }

      return {};
    }
  };

  template <typename ... T>
  struct one_of;

  template <typename Rule, typename ... Rest>
  struct one_of<Rule, Rest ...>
  {
    static state parser_function(loader::file_position & pos, parser_data * data)
    {
      state status = Rule::parser_function(pos, data);

      if( status.message.size() > 0 )
      {
        return one_of<Rest ...>::parser_function(pos, data);
      }

      return status;
    }
  };

  template <typename Rule>
  struct one_of<Rule>
  {
    static state parser_function(loader::file_position & pos, parser_data * data)
    {
      return Rule::parser_function(pos, data);
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
    static std::string add_constant_to_module(parser_data * data)
    {
      sys::literal name = data->dict.add( std::move(data->info.cell_name) );
      try
      {
        data->current_module->constants.append_row<configuration::module_config::read_only_cell>();
      }
      catch( lib::table_key_not_unique const & )
      {
        return std::format("{} {} {}", data->current_module->name->first, "module already have such cell name"sv, data->info.cell_name);
      }

      return {};
    }

    static std::string add_module(sys::string & name, parser_data * data)
    {
      sys::literal module_name = data->dict.add( std::move(name) );
      data->current_module = data->modules.append_row<type_system::meta::meta_information>(module_name).result;

      return {};
    }

    static std::string set_cell_name(sys::string & name, parser_data * out)
    {
      out->info.cell_name = std::move(name);
      return {};
    }

    static std::string set_number(sys::string & digits, parser_data * out)
    {
      out->info.number = inner::number_from_string(digits);
      return {};
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

  using lib::static_text;

  template <static_text st>
  struct is_text
  {
    bool recognzie(loader::file_position & pos)
    {
      typename std::string::const_iterator it = pos.current;

      for( char ch : st.text )
      {
        if( ch == *(it++) ) { continue; }
        if( pos.last == pos.current ) return false;
      }

      pos.current = it;

      return true;
    }
  };

  using fn_type = decltype(&actions::add_module);
  using namespace lib::static_text_literal;

  struct boolean_yes : is_text<"yes"_st> {};
  struct boolean_no : is_text<"no"> {};

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

  template <char FirstChar>
  struct keyword
  {
    sys::string result;

    bool recognize(loader::file_position & position)
    {
      if( *position.current != FirstChar ) { return false; }

      loader::file_position new_position{ position.current + 1, position.last };

      while( std::isalpha(*new_position.current) )
      {
        if( new_position.current == new_position.last ) { return false; }
        ++new_position.current;
      }

      position = new_position;
      result.assign(position.current, position.last);

      return true;
    }
  };

  struct module_name : keyword<'@'>
  {
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

  struct module_add_constant
  {
    static state parser_function(loader::file_position & pos, parser_data * data);
  };

  struct literal : keyword<'`'> {};

  struct add_literal_from_string_to_sequence
  {
    static state fn(std::string const & pos, parser_data * data)
    {
      //
      return {};
    }
  };

  struct module_start_constant_initialization : one_of<
    sentence<literal, add_literal_from_string_to_sequence>,
    rule<number, module_add_constant>
  > {};

  struct after_module_cell_name : rule<colon, module_start_constant_initialization>
  {};

  struct inside_module : rule<variable_name, after_module_cell_name>
  {
  };

  struct inside_file : rule<module_name, inside_module>
  {};


  inline state module_add_constant::parser_function(loader::file_position & pos, parser_data * data)
  {
    return { actions::add_constant_to_module(data), &inside_module::parser_function };
  }

}
namespace ksi::interpreter::loader
{

  struct script_parser : parser_data
  {
    script_parser(vm_config * vm_configuration) : parser_data{ vm_configuration } {}

    static bool is_space(sys::character ch) { return (ch == ' ' || ch == '\n'); }

    void parse(sys::string const & file_content)
    {
      file_position pos{ file_content.cbegin(), file_content.cend() };

      state status{ {}, &rules::inside_file::parser_function };

      while( (pos.current != pos.last) && status.message.empty() )
      {
        skip_space(pos.current);
        status = status.current_function(pos, this);
      }

      if( status.message.size() ) {}
    }
  };

  struct script_loader
  {
    script_parser parser;

    script_loader(vm_config * vm_configuration) : parser{ vm_configuration } {}
  };

}