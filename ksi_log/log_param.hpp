#pragma once

namespace ksi::log {


  template <typename Info, typename Source>
  struct log_param
  {
    Info    info;
    Source  source;
  };


  template <typename Type, typename String, typename Code>
  struct log_info
  {
    Type    type;
    String  message;
    Code    code;
  };


  template <typename Path, typename Position>
  struct script_source
  {
    Path      path;
    Position  position;
  };


} // ns
