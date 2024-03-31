#pragma once

  #include <cstddef>

namespace ksi::log {


  template <typename Path, typename Position>
  struct script_source
  {
    Path      path;
    Position  position;
  };


  using index_type = std::ptrdiff_t;


  struct file_position
  {
    index_type  char_index;
    index_type  line;
    index_type  column;
  };


} // ns
