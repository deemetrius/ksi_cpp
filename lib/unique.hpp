#pragma once

namespace ksi::lib
{

  template <typename T>
  struct deleter
  {
    static void close(T * handle) { delete handle; }

    decltype(&close) fn{ &close };

    void operator () (T * h) const { fn(h); }

    deleter() = default;
    deleter(std::default_delete<T> &&) noexcept {}
  };

  template <typename T>
  using unique = std::unique_ptr< T, deleter<T> >;

}
