#pragma once

  #include "ksi_log/basic_log.hpp"
  #include <source_location>
  #include "ksi_log/file_position.hpp"

namespace ksi::interpreter {


  struct log
  {
    enum message_type { is_notice, is_error };
    using code_type = std::int32_t;

    template <typename String>
    using internal = ksi::log::basic_log<message_type, String, code_type, std::source_location>;

    template <typename String, typename Path>
    using for_script = ksi::log::basic_log<
      message_type, String, code_type,
      ksi::log::script_source<Path, ksi::log::file_position>
    >;
  };


} // ns
