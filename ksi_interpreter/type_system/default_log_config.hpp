#pragma once

  #include "ksi_log/log_makers.hpp"

namespace ksi::interpreter {


  // none, file, console, /*memory*/


  struct default_log_config
  {
    //static constexpr ksi::log::maker_none   log_maker_script;

    struct internal_log_params
    {
      static constexpr std::string_view
        format_message{ "{} #{}:\n{}\n" },
        format_source_location{ "[{}:{}] {}\n\n" }
      ;
    };
    static inline const ksi::log::maker_to_file<internal_log_params>  log_maker_internal{ "log.txt" };
  };


} // ns
