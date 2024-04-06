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

  enum class log_message_level { notice, error };
  // todo: enum serialization // binding & output

} // ns


template<>
struct std::formatter<ksi::interpreter::log_message_level, char>
{
  template <typename ParseContext>
  constexpr ParseContext::iterator parse(ParseContext & ctx)
  {
    /* auto it = ctx.begin();
    if( it == ctx.end() ) { return it; }
    if( *it != '}' ) { throw std::format_error("Invalid format args for log_message_level."); }
    return it; */
    return ctx.begin();
  }

  template <typename FmtContext>
  FmtContext::iterator format(ksi::interpreter::log_message_level s, FmtContext & ctx) const
  {
    using namespace std::string_view_literals;

    std::string_view  text[]{"notice"sv, "error"sv};
    return std::ranges::copy( text[static_cast<std::size_t>(s)], ctx.out() ).out;
  }
};


namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::log
  {
    using code_type = std::int32_t;

    struct message
    {
      t_string            text;
      log_message_level   type;
      code_type           code;
    };

    struct position_in_file
    {
      t_path_view               path;
      ksi::log::file_position   pos_info;
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

    using t_format_message = std::format_string<log_message_level const &, code_type const &, std::string>;

    template <typename Record>
    struct writer_fn
    {
      // message: type, code, text

      t_format_message   format_message;
      //format_source_location;

      void operator() (ksi::files::file_handle::handle_type file_handle, Record const & record) const
      {
        std::print(file_handle, format_message,
          record.info->type,
          record.info->code,
          converter_string_print(record.info->text)
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
