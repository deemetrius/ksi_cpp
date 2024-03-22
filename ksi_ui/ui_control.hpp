#pragma once

#include "ui_system.hpp"
#include <memory>

namespace ui {


  struct control_base
  {
    system::control_type type;
  };


  using control_pointer = std::shared_ptr<control_base>;
  using control_handle = control_base *;


  template <typename Control>
  struct control : public control_base
  {
    using self_pointer = std::shared_ptr<control>;

    static inline const system::control_type
      self_type = system::reg_control_type(Control::type_name)
    ;

    Control params;

    control() : control_base{ self_type }
    {}
  };


  template <typename Control>
  using control_pointer_for = std::shared_ptr< control<Control> >;


} // ns
