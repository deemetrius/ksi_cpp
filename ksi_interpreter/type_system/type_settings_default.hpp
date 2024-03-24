#pragma once

#include "ksi_lib/conv.string.hpp"
#include <cstdint>
#include <string>

namespace ksi::interpreter {


  /*
    Do not midify this file

    To specify your own type_settings:
    * Create new header file somewhere in your project dir
    * Copy this struct to the header file you made
    * Change type aliases (after =) to desired types
    * Pass your type_settings struct as template parameter to nest "system"

    (example) ksi::interpreter::system< your_settings >
  */

  struct type_settings_default
  {
    using t_bool      = bool;
    using t_integer   = std::intptr_t;
    using t_floating  = double;
    using t_string    = std::wstring;

    static constexpr ksi::conv::string_cast<t_string> converter_string;

    // currently: we do not call destructors of placed values (such as: value_bool, value_int, value_float)
    static constexpr bool call_destructor_for_simple_placed_values{ false };
  };


} // ns
