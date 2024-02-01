#pragma once

/*
  wrapper around: std::basic_regex

  https://en.cppreference.com/w/cpp/regex/basic_regex/basic_regex

  According to given documentation:
    default constructor of std::basic_regex
    makes pattern that matches nothing
*/

// ^(?!0)0

#include "conv.string.hpp"
#include <regex>
#include <optional>

namespace ksi::lib {


  using namespace std::string_literals;

  template <typename String_type>
  struct regex_nest
  {
    using string_type = String_type;
    using char_type = string_type::value_type;
    using regex_type = std::basic_regex<char_type>;
    using size_type = string_type::size_type;
    using option_type = regex_type::flag_type;
    using error_code_type = std::regex_constants::error_type;

    static constexpr option_type default_option{ regex_type::ECMAScript };

    using conv_type = conv::from_string::to<string_type>;
    static constexpr conv_type converter{};

    static inline const string_type option_chars{ converter("i"s) };

    static option_type make_options(const string_type & flags)
    {
      option_type ret{ default_option };
      if( flags.find(option_chars[0]) != flags.npos )
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

    static string_type filter_unique(string_type const & where, string_type const & keep_chars)
    {
      if( where.size() == 0 ) { return where; }
      if( keep_chars.size() == 0 ) { return keep_chars; }

      size_type count{ 0 };
      std::vector<std::uint8_t> mask(where.size(), 0);
      for( char_type ch : keep_chars )
      {
        size_type pos = where.find(ch);
        if( pos != where.npos )
        {
          mask[pos] = 1;
          ++count;
        }
      }

      string_type ret(count, 0);
      size_type pos{ 0 };
      for( size_type i = 0, size = where.size(); i < size; ++i )
      {
        if( mask[i] == 1 )
        {
          ret[pos] = where[i];
          ++pos;
        }
      }

      return ret;
    }

    struct error_info_type
    {
      error_code_type code;
      string_type message;
    };

    using opt_error_info_type = std::optional<error_info_type>;

    struct data_pattern
    {
      enum kind {
        type_regular,
        type_exact,
        type_prefix,
        type_ending,
        type_mask
      };

      enum status_type {
        status_not_ready = 0,
        status_mistake = -1,
        status_ok = 1,
      };

      // props

      string_type   source_string;
      kind          type;

      string_type   pattern_string;
      string_type   mode_chars;

      status_type   status{ status_not_ready };
      regex_type    regex{};

      bool is_fine() const
      {
        return (status == status_ok);
      }

      bool init(opt_error_info_type & maybe_error)
      {
        status = status_not_ready;
        option_type opts{ make_options(mode_chars) };
        try
        {
          regex = regex_type{ pattern_string, opts };
          status = status_ok;
        }
        catch( const std::regex_error & e )
        {
          status = status_mistake;
          maybe_error = { e.code(), converter( e.what() ) };
          return false;
        }
        return true;
      }
    };

    struct pattern
      : public data_pattern
    {
      static inline thread_local opt_error_info_type error_info{};

      // actions

      bool match(string_type const & subject)
      {
        return std::regex_search(subject, this->regex, std::regex_constants::match_any);
      }

      // may throw: std::regex_error
      string_type replace(const string_type & subject, const string_type & replacement)
      {
        return std::regex_replace(subject, this->regex, replacement);
      }

      // pattern makers

      static pattern regular(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_regular,
          source_string,
          filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern exact(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_exact,
          converter("^") + escape(source_string) + converter("$"),
          filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern prefix(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_prefix,
          converter("^") + escape(source_string),
          filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern ending(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_ending,
          escape(source_string) + converter("$"),
          filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static string_type escape(const string_type & str)
      {
        string_type what{ converter(R"(\^$.*+?()[]{}|-)"s) };
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
    }; // end pattern

    struct maybe_pattern
    {
      pattern pattern;
      opt_error_info_type maybe_error{};

      static maybe_pattern regular_try(const string_type & source_string, const string_type & mode_chars)
      {
        maybe_pattern ret{{
          source_string,
          pattern::type_regular,
          source_string,
          filter_unique(mode_chars, option_chars)
        }};
        ret.pattern.init(ret.maybe_error);
        return ret;
      }
    };

  }; // end nest


} // end ns
