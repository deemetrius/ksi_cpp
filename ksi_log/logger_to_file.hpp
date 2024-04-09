#pragma once

  #include "file.hpp"
  #include <string>
  #include "i_log.hpp"

namespace ksi::log {


  template <typename Record, typename Writer>
  struct logger_to_file
    : public i_log<Record>
  {
    using writer_type = Writer::template functor<Record>;

    // props
    std::string   file_path;
    writer_type   writer;

    logger_to_file(std::string path, writer_type && writer_params)
      : file_path{ std::move(path) }
      , writer{ std::move(writer_params) }
    {}

    void add(Record record) override
    {
      files::file_handle file{
        std::fopen(file_path.c_str(), files::open_modes::open_write_new_or_append.text.internal)
      };
      writer(file.handle, record);
    }
  };


} // ns
