#pragma once

namespace ksi::lib {


  template <typename Type>
  struct counter
  {
    Type value;

    Type back() { return (value -= 1); }
    Type operator () () { return value++; }
    Type operator () (Type set) { return (value = set); }
  };


} // ns
