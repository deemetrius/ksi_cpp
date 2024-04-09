#pragma once

namespace ksi::type_actions {


  template <typename ... Bases>
  struct mix_of
    : public Bases ...
  {};


} // ns
