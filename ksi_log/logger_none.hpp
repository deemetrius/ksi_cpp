#pragma once

  #include "i_log.hpp"

namespace ksi::log {


  template <typename Record>
  struct logger_none : public i_log<Record>
  {
    void add(Record record) override {}
  };


} // ns
