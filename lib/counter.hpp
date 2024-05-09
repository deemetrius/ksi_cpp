#pragma once

namespace ksi::lib {


  template <typename Type>
  struct counter
  {
    Type value{};

    Type operator () () { return value++; }
  };


} // ns
