#pragma once

#include "conv.string.hpp"
#include <regex>

namespace ksi::lib {

  template <typename String_type>
  struct regex_nest
  {
    using string_type = String_type;
    using char_type = string_type::value_type;
    using regex_type = std::basic_regex<char_type>;
    using size_type = string_type::size_type;
    using option_type = regex_type::flag_type;

    using conv_type = conv::from_string::to<string_type>;

    static constexpr option_type option{ regex_type::ECMAScript };

    static option_type make_option(const string_type & flags)
    {
      string_type opts{ conv_type{}("i") };
      option_type ret{ option };
      if( flags.find(opts[0]) != flags.npos )
      {
        ret |= regex_type::icase;
      }
      return ret;
    }

    static size_type count_presence(const string_type & where, const string_type & what)
    {
      size_type ret{ 0 };
      for( char_type ch : where )
      {
        if( what.find(ch) != what.npos )
        {
          ++ret;
        }
      }
      return ret;
    }

    static string_type escape(const string_type & str)
    {
      string_type what{ conv_type{}("\\^$.*+?()[]{}|-") };
      size_type size{ count_presence(str, what) + str.size() };
      string_type ret{ size, char_type{0}, typename string_type::allocator_type{} };
      size_type i{0};
      for( char_type ch : str )
      {
        if( what.find(ch) != what.npos )
        {
          ret[i] = what[0];
          ++i;
        }
        ret[i] = ch;
        ++i;
      }
      return ret;
    }

    static regex_type regex_never()
    {
      return regex_type(conv_type{}("^(?!x)x"), option);
    }

    struct pattern
    {
      enum kind {
        type_regular,
        type_exact,
        type_prefix,
        type_ending,
        type_mask
      };

      enum code {
        code_fine,
        code_mistake, // wrong pattern
        code_error
      };


      string_type regex;
      string_type value;
      string_type mode;
      kind type;
      code status{ code_fine };
      string_type msg;


      void reset_status()
      {
        status = code_fine;
        msg.clear();
      }


      regex_type make_regex()
      {
        reset_status();
        option_type opt{ make_option(mode) };
        try
        {
          return regex_type{ regex, opt };
        }
        catch( const std::regex_error & e )
        {
          status = code_mistake;
          msg = conv_type{}( e.what() );
          return regex_never();
        }
      }

      bool match(const string_type & subject)
      {
        reset_status();
        regex_type re{ make_regex() };
        try
        {
          return std::regex_search(subject, re, std::regex_constants::match_any);
        }
        catch( const std::regex_error & e )
        {
          status = code_error;
          msg = conv_type{}( e.what() );
          return false;
        }
      }

      string_type replace(const string_type & subject, const string_type & replacement)
      {
        reset_status();
        regex_type re{ make_regex() };
        try
        {
          return std::regex_replace(subject, re, replacement);
        }
        catch( const std::regex_error & e )
        {
          status = code_error;
          msg = conv_type{}( e.what() );
          return conv_type{}("");
        }
      }


      static pattern regular(const string_type & str, const string_type & flags)
      {
        return {
          str,
          str,
          flags,
          type_regular
        };
      }

      static pattern exact(const string_type & str, const string_type & flags)
      {
        return {
          conv_type{}("^") + escape(str) + conv_type{}("$"),
          str,
          flags,
          type_exact
        };
      }

      static pattern prefix(const string_type & str, const string_type & flags)
      {
        return {
          conv_type{}("^") + escape(str),
          str,
          flags,
          type_exact
        };
      }

      static pattern ending(const string_type & str, const string_type & flags)
      {
        return {
          escape(str) + conv_type{}("$"),
          str,
          flags,
          type_exact
        };
      }

    }; // end pattern

  }; // end nest

} // end ns
