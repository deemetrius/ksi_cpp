#pragma once

#include <memory>

namespace ksi::lib
{

  template <typename T>
  struct inner_deleter
  {
    static void close(T * h) { h->fn(h); }

    inner_deleter() = default;
    inner_deleter(std::default_delete<T>) {}
  };

  template <typename T>
  using unique_inner = std::unique_ptr<T, inner_deleter<T> >;

}