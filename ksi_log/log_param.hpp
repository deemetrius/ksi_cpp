#pragma once

namespace ksi::log {


  template <typename Info, typename Source>
  struct record
  {
    using info_type = Info;
    using source_type = Source;
    using info_storage = Info const *;

    info_storage  info;
    source_type   source;
  };


  template <typename Type, typename String, typename Code>
  struct message_info
  {
    Type    type;
    String  message;
    Code    code;
  };


} // ns
