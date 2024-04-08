#pragma once

  #include "file.hpp"
  #include <string>
  #include "ksi_lib/one_of.hpp"

namespace ksi::log {


  template <typename Interface, typename Writer, files::std_marker Target = files::std_marker::std_output>
    requires( lib::one_of(Target, files::std_marker::std_output, files::std_marker::std_error) )
  struct logger
    : public Interface
  {
    using typename Interface::record_type;

    // props
    Writer        writer;

    logger(Writer && writer_params)
      : writer{ std::move(writer_params) }
    {}

    void add(record_type record) override
    {
      writer(stdout, record);
    }
  };


} // ns
