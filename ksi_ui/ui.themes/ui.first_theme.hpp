#pragma once

#include "../ui_theme.hpp"
#include "../ui.controls/ui.casual_controls.hpp"

  #include <iostream> // test

namespace ui::themes {


  struct first_theme
  {
    using supported_controls = supported<controls::Button, controls::Label>;

    struct theme_config
    {
      system::string_type theme_name;
    };

    template <typename Control>
    struct instance
    {
      theme_config config;

      using param_type = control<Control> *;

      void draw(param_type ctl);
    };
  };


  template <>
  inline void first_theme::instance<controls::Button>::draw(param_type ctl)
  {
    std::cout << '"' << ctl->type->name << "\" drawn special";
    std::cout << " using " << config.theme_name << " theme\n";
  }


  template <typename Control>
  inline void first_theme::instance<Control>::draw(param_type ctl)
  {
    std::cout << config.theme_name << " theme draws '" << ctl->type->name << "' control\n";
  }


} // ns
