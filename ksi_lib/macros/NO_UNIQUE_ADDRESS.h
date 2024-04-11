#pragma once

#ifndef NO_UNIQUE_ADDRESS
  #ifdef __has_cpp_attribute
    #if __has_cpp_attribute(no_unique_address)
      #if defined(__clang__)
        #define NO_UNIQUE_ADDRESS __attribute__((__no_unique_address__))
      #elif defined(_MSC_VER)
        #define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
      #else
        #define NO_UNIQUE_ADDRESS [[no_unique_address]]
      #endif
    #endif
  #endif
#endif

#ifndef NO_UNIQUE_ADDRESS
  #define NO_UNIQUE_ADDRESS
#endif