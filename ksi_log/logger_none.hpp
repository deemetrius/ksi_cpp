#pragma once

namespace ksi::log {


  template <typename Interface>
  struct logger_none
    : public Interface
  {
    using typename Interface::record_type;

    void add(record_type record) override
    {}
  };


} // ns
