#pragma once

namespace ksi::log {


  template <typename Record>
  struct i_log
  {
    using record_type = Record;

    virtual void add(record_type record) = 0;

    // dtor
    virtual ~i_log() = default;
  };


} // ns
