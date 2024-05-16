#pragma once
#include <vector>
#include <print>
namespace ksi::lib
{
  template <typename T>
  void show_vector(const T & vec)
  {
    std::print("-\n");
    for( auto & i : vec )
    {
      std::print("{}\n", i.name);
    }
  }
}