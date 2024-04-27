#pragma once

namespace ksi::lib::traits {

  struct no_copyable {
    no_copyable() = default;
    no_copyable(no_copyable const &) = delete;
    no_copyable(no_copyable &&) = delete;
  };

}