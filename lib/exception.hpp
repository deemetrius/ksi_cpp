#pragma once

#include <exception>
#include <utility>

namespace ksi::lib
{

  struct exception
  {
    static inline std::exception_ptr ptr;
    static inline std::size_t skip_count{ 0 };

    static void save()
    {
      if( ptr ) { ++skip_count; return; }
      ptr = std::current_exception();
    }

    static void rethrow()
    {
      if( ! ptr ) { return; }
      std::rethrow_exception(
        std::exchange(ptr, std::exception_ptr{})
      );
    }
  };

}
