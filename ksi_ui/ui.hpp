#pragma once

#include "ui.themes/ui.first_theme.hpp"

namespace ui {


  template <typename Control>
  struct fn_make
  {
    using value_type = control<Control>;
    using result_type = std::shared_ptr<value_type>;

    constexpr result_type operator () (Control params) const
    {
      result_type ret = std::make_shared<value_type>();
      ret->params = std::move(params);
      return ret;
    }
  };

  template <typename Control>
  constexpr fn_make<Control> make{}; // variable templates (since C++14)
  // see: https://en.cppreference.com/w/cpp/language/variable_template


} // ns
