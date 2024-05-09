#pragma once

namespace ksi::lib
{

   template <typename T>
  struct deleter_base
  {
    static void close(T * handle) { delete handle; }

    decltype(&close) fn{ &close };
  };

  template <typename T>
  struct deleter : deleter_base<T>
  {
    void operator () (T * h) const { this->fn(h); }

    deleter() = default;
    deleter(std::default_delete<T> &&) noexcept {}
  };

  template <typename T>
  using unique = std::unique_ptr< T, deleter<T> >;

}
