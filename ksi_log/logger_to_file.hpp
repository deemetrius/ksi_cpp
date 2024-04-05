#pragma once

  #include "i_log.hpp"
  #include "file.hpp"
  #include <string>

namespace ksi::log {


  template <typename Interface, typename Writer>
  struct logger_to_file
    : public Interface
  {
    using typename Interface::record_type;

    // props
    std::string   file_path;
    Writer        writer;

    logger_to_file(std::string path, Writer && writer_params)
      : file_path{ std::move(path) }
      , writer{ std::move(writer_params) }
    {}

    void add(record_type record) override
    {
      files::file_handle file{ std::fopen(file_path.c_str(), files::open_modes::open_write_new_or_append.text.internal) };
      writer(file.handle, record);
    }
  };


} // ns
