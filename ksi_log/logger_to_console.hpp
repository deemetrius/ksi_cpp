#pragma once

  #include "file.hpp"
  #include <string>

namespace ksi::log {


  template <typename Interface, typename Writer>
  struct logger_to_file
    : public Interface
  {
    using typename Interface::record_type;

    // props
    Writer        writer;

    logger_to_file(Writer && writer_params)
      : writer{ std::move(writer_params) }
    {}

    void add(record_type record) override
    {
      writer(stdout, record);
    }
  };


} // ns
