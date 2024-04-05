#pragma once

  #include <cstddef>

namespace ksi::log {


  using index_type = std::ptrdiff_t;


  struct file_position
  {
    index_type  char_index;
    index_type  line;
    index_type  column;
  };


} // ns
