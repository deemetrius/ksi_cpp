#pragma once

#include "../ui_control.hpp"

namespace ui::controls {


  struct Button
  {
    static inline system::string_type type_name = "ui.Button";

    system::string_type caption;
  };


  struct Label
  {
    static inline system::string_type type_name = "ui.Label";

    system::string_type text;
    control_pointer     for_control{};
  };


} // ns
