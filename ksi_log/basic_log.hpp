#pragma once

  #include "log_param.hpp"
  #include "i_log.hpp"

namespace ksi::log {


  template <
    typename Type, typename String, typename Code,
    typename Source
  >
  using basic_log = i_log< record<
    message_info<Type, String, Code>,
    Source
  > >;


} // ns
