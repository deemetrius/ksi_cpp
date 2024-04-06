#pragma once

  #include "../system_nest.hpp"

  #include "ksi_log/i_log.hpp"
  #include "ksi_log/file_position.hpp"
  #include <source_location>
  #include <print>

  #include <memory>

  #include "ksi_log/logger_to_file.hpp"
  #include "ksi_log/logger_none.hpp"

namespace ksi::interpreter {

  enum class log_level { info, error, notice };
  // todo: enum serialization // binding & output

} // ns


template<>
struct std::formatter<ksi::interpreter::log_level, char>
{
  template <typename ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & ctx)
  {
    /* auto it = ctx.begin();
    if( it == ctx.end() ) { return it; }
    if( *it != '}' ) { throw std::format_error("Invalid format args for log_level."); }
    return it; */
    return ctx.begin();
  }

  template <typename FmtContext>
  FmtContext::iterator format(ksi::interpreter::log_level s, FmtContext & ctx) const
  {
    using namespace std::string_view_literals;

    std::string_view  text[]{"info"sv, "error"sv, "notice"sv};
    return std::ranges::copy( text[static_cast<std::size_t>(s)], ctx.out() ).out;
  }
};


namespace ksi::interpreter {

  namespace detail {
    template <typename ... Args> std::format_string<Args ...> print_format_helper(Args && ...);
  } // ns


  template <typename Type_settings>
  struct system<Type_settings>::log
  {
    struct messages;
    struct keys;

    using code_type = std::int32_t;

    struct message_key
    {
      log_level   level;
      code_type   code;
    };

    struct message
    {
      t_string      text;
      message_key   type;
    };

    struct position_in_file
    {
      t_path_view               path;
      ksi::log::file_position   pos_info;

      t_path_view file_name() const { return path; }
      ksi::log::index_type line() const { return pos_info.line; }
      ksi::log::index_type column() const { return pos_info.column; }
    };

    using ptr_message = message const *;

    struct script_record_type
    {
      ptr_message       info;
      position_in_file  source_location;
    };

    struct internal_record_type
    {
      ptr_message           info;
      std::source_location  source_location;
    };

    using internal_interface = ksi::log::i_log<typename log::internal_record_type>;
    using internal_interface_ptr = internal_interface *;
    using script_interface = ksi::log::i_log<typename log::script_record_type>;

    // message: level, code, text
    using t_format_message = std::format_string<log_level const &, code_type const &, std::string>;

    template <typename Record>
    struct writer_fn
    {
      using t_format_location = decltype( detail::print_format_helper(
        std::declval<Record const &>().source_location.line(),
        std::declval<Record const &>().source_location.column(),
        std::declval<Record const &>().source_location.file_name()
      ) );

      t_format_message    format_message;
      t_format_location   format_source_location;

      void operator() (ksi::files::file_handle::handle_type file_handle, Record const & record) const
      {
        std::print(file_handle, format_message,
          record.info->type.level,
          record.info->type.code,
          converter_string_print(record.info->text)
        );
        std::print(file_handle, format_source_location,
          record.source_location.line(),
          record.source_location.column(),
          record.source_location.file_name()
        );
      }
    };

    using internal_writer_fn = writer_fn<internal_record_type>;

    using internal_log_holder = std::shared_ptr<internal_interface>;
    using internal_logger_to_file = ksi::log::logger_to_file<internal_interface, internal_writer_fn>;
    using internal_logger_to_file_holder = std::shared_ptr<internal_logger_to_file>;

    using internal_logger_none = ksi::log::logger_none<internal_interface>;

    static internal_logger_to_file_holder
    internal_logger_to_file_make(std::string file_path, writer_fn<internal_record_type> && writer)
    {
      return std::make_shared<internal_logger_to_file>( std::move(file_path), std::move(writer) );
    }
  };


} // ns
