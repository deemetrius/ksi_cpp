#pragma once

#include <print>

namespace ksi::lib
{

  template <typename Types>
  void show_table(Types & nest)
  {
    for( auto ptr : nest.pos )
    {
      std::print("{:3}  {}\n", ptr->id, ptr->name->first);
    }
  }

}