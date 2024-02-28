#pragma once

#include <string>

namespace ksi::interpreter::errors {


  struct base { std::string msg; };
  struct internal : public base {};
  struct method_not_supported : public internal {};
  struct release_on_empty : public internal {};


} // ns
