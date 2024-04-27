#pragma once

  #include "log_system.hpp"

namespace ksi::interpreter::log_messages {


  using log_system::message_type;

  struct startup_messages {
    static inline log_system::log_message const

    intro{
      "system started", message_type::information, 1
    }

    ,

    sys_type_name_not_unique{
      "system type name must be unique", message_type::error_message, 2
    }

    ;
  };


}
