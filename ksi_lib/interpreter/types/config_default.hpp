#pragma once

#include "ksi_lib/conv.string.hpp"
#include <cstdint>
#include <string>

namespace ksi::interpreter {


  /*
    Do not midify this file

    To specify your own config:
    * Create new header file somewhere in your project dir
    * Copy this struct to the header file you made
    * Change type aliases (after =) to desired types
    * Pass your config struct as template parameter to nest "types"
    * (example) ksi::interpreter::types< your_config >
  */

  struct type_config
  {
    using t_bool      = bool;
    using t_integer   = std::intptr_t;
    using t_floating  = double;
    using t_string    = std::wstring;

    static constexpr ksi::conv::string_cast<t_string> converter_string;
  };


} // ns
