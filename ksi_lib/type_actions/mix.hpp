#pragma once

namespace ksi::type_actions {


  template <typename ... Bases>
  struct mix
    : public Bases ...
  {};


} // ns
