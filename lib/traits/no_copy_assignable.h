#pragma once

namespace ksi::lib::traits {
  struct no_copy_assignable
  {
    no_copy_assignable & operator = (const no_copy_assignable &) = delete;
  };
}