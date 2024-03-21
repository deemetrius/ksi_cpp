#pragma once

#include "../ui_theme.hpp"
#include "../ui.controls/ui.casual_controls.hpp"

  #include <iostream> // test

namespace ui::themes {


  struct first_theme
  {
    using supported_controls = supported<controls::Button, controls::Label>;

    template <typename Control>
    struct instance
    {
      using param_type = control<Control> *;

      void draw(param_type ctl);
    };
  };


  template <typename Control>
  inline void first_theme::instance<Control>::draw(param_type ctl)
  {
    std::cout << ctl->type->term << " drawn\n";
  }


  template <>
  inline void first_theme::instance<controls::Button>::draw(param_type ctl)
  {
    std::cout << ctl->type->term << " drawn special\n";
  }


} // ns
