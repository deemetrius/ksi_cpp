#pragma once

  #include <string>
  #include <cstdio>
  #include <string_view>
  #include <print>
  #include <source_location>
  #include "lib/auxiliary/log_console.hpp"

namespace ksi::interpreter::log_system {


  enum class message_type {
    information   = 0,
    error_message = 1,
    notice        = 2
  };

  struct log_message
  {
    std::string   text;
    message_type  type;
    std::size_t   code;
  };

  struct log_writer
  {
    static constexpr std::string_view  format_location{ "\n[{}:{}] {}\n" };
    static constexpr std::string_view   format_message{ "{} #{}\n{}\n" };

    static void write_message(std::FILE * to, const log_message & message)
    {
      std::string_view types[]{ "info", "error", "notice" };
      std::print(to, format_message, types[static_cast<std::size_t>(message.type)], message.code, message.text);
    }

    static void show_location(std::FILE * to, std::source_location where)
    {
      std::print(
        to, format_location,
        where.line(), where.column(), where.file_name()
      );
    }
  };

  using to_console = lib::auxiliary::log_console<std::source_location, log_message const, log_writer>;
  using log_pointer = to_console::base_type *;
  //using log_pointer = lib::auxiliary::i_log<std::source_location, log_message const> *;


} // ns