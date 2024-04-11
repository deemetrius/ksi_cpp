#pragma once

namespace ksi::type_actions {


  struct none
  {
    template <typename T> none(T) {}
    none() = default;
  };


} // ns
