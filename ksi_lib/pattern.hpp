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
#include <string_view>
#include <span>
#include <vector>

namespace ksi::chars {


  template <typename String_type>
  inline auto count_presence(const String_type & where, const String_type & what) -> typename String_type::size_type
  {
    using size_type = typename String_type::size_type;
    using char_type = typename String_type::value_type;

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


  template <typename String_type>
  inline String_type filter_unique(String_type const & where, String_type const & keep_chars)
  {
    using size_type = typename String_type::size_type;
    using char_type = typename String_type::value_type;

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

    String_type ret(count, 0);
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


  template <typename Char_type>
  static bool is_word_char(const Char_type ch)
  {
    using namespace std::string_literals;
    using string_type = std::basic_string<Char_type>;
    using conv_type = conv::from_string::to<string_type>;

    static const std::basic_regex<Char_type> regex{ conv_type{}(R"(\w)"s) };

    return std::regex_match(&ch, &ch + 1, regex);
  }


  template <typename Char_type>
  static bool is_new_line(const Char_type ch)
  {
    if constexpr( std::is_same_v<Char_type, char> )
    {
      return (ch == '\n');
    }
    else if constexpr( std::is_same_v<Char_type, wchar_t> )
    {
      return (ch == L'\n');
    }
  }


} // end ns

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

    static inline thread_local option_type default_option{ regex_type::ECMAScript };

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

    static string_type escape_pattern_string(const string_type & str)
    {
      string_type special{ converter(R"(\^$.*+?()[]{}|-)"s) };
      size_type size{ chars::count_presence(str, special) + str.size() };
      string_type ret{ size, char_type{0}, typename string_type::allocator_type{} };
      size_type i{0};
      for( char_type ch : str )
      {
        if( special.find(ch) != special.npos )
        {
          ret[i] = special[0];
          ++i;
        }
        ret[i] = ch;
        ++i;
      }
      return ret;
    }

    struct match_range
    {
      using find_next_result = std::vector<match_range>;

      size_type from;
      size_type to;

      size_type length() const
      {
        return (to - from);
      }

      std::basic_string_view<char_type> make_view(string_type const & source_string) const
      {
        return {source_string.cbegin() + from, source_string.cbegin() + to};
      }

      std::span<char_type> make_span(string_type & source_string) const
      {
        return {source_string.begin() + from, source_string.begin() + to};
      }

      bool last_word_char(string_type const & source_string) const
      {
        if( to == 0 ) { return false; }
        return chars::is_word_char(source_string[to - 1]);
      }

      bool last_new_line(string_type const & source_string) const
      {
        if( to == 0 ) { return true; }
        return chars::is_new_line(source_string[to - 1]);
      }

      find_next_result find_next(regex_type const & regex, string_type const & source_string) const
      {
        if( to >= source_string.size() ) { return {}; }
        using results_type = std::match_results<typename string_type::const_iterator>;
        results_type results;

        std::regex_constants::match_flag_type flags = std::regex_constants::match_not_bol;
        if( last_new_line(source_string) )
        {
          flags = std::regex_constants::match_default;
        }
        else if( last_word_char(source_string) == chars::is_word_char(source_string[to]) )
        {
          flags |= std::regex_constants::match_not_bow;
        }

        if(
          std::regex_search(
            source_string.cbegin() + to,
            source_string.cend(),
            results,
            regex,
            flags
          ) == false
        ) { return {}; }
        find_result ret;
        ret.reserve( results.size() );
        for( typename results_type::const_reference it : results )
        {
          ret.emplace_back(
            it.first - source_string.cbegin(),
            it.second - source_string.cbegin()
          );
        }
        return ret;
      }
    };

    using find_result = std::vector<match_range>;
    using search_result = std::vector<find_result>;

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

      bool match(string_type const & subject) const
      {
        return std::regex_search(subject, this->regex, std::regex_constants::match_any);
      }

      // may throw: std::regex_error
      string_type replace(const string_type & subject, const string_type & replacement) const
      {
        return std::regex_replace(subject, this->regex, replacement);
      }

      find_result find_first(string_type const & subject) const
      {
        using results_type = std::match_results<typename string_type::const_iterator>;
        results_type results;
        if( std::regex_search(subject, results, this->regex) == false ) { return {}; }
        find_result ret;
        ret.reserve( results.size() );
        for( typename results_type::const_reference it : results )
        {
          ret.emplace_back(
            it.first - subject.cbegin(),
            it.second - subject.cbegin()
          );
        }
        return ret;
      }

      search_result full_search(string_type const & subject) const
      {
        search_result ret;
        find_result current = find_first(subject);
        while( current.size() > 0 )
        {
          ret.push_back(
            std::exchange(
              current,
              current[0].find_next(this->regex, subject)
            )
          );
        }
        return ret;
      }

      // pattern makers

      static pattern make_regular(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_regular,
          source_string,
          chars::filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern make_exact(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_exact,
          converter("^") + escape_pattern_string(source_string) + converter("$"),
          chars::filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern make_prefix(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_prefix,
          converter("^") + escape_pattern_string(source_string),
          chars::filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }

      static pattern make_ending(const string_type & source_string, const string_type & mode_chars)
      {
        pattern ret{
          source_string,
          pattern::type_ending,
          escape_pattern_string(source_string) + converter("$"s),
          chars::filter_unique(mode_chars, option_chars)
        };
        error_info.reset();
        ret.init(error_info);
        return ret;
      }
    }; // end pattern

    struct maybe_pattern
    {
      pattern pattern;
      opt_error_info_type maybe_error{};

      bool is_fine() const
      {
        return (maybe_error.has_value() == false);
      }

      static maybe_pattern make_regular_try(const string_type & source_string, const string_type & mode_chars)
      {
        maybe_pattern ret{{
          source_string,
          pattern::type_regular,
          source_string,
          chars::filter_unique(mode_chars, option_chars)
        }};
        ret.pattern.init(ret.maybe_error);
        return ret;
      }
    };

  }; // end nest


} // end ns
