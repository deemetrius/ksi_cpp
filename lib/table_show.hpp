#pragma once

#include <print>

namespace ksi::lib
{

  template <typename Types>
  void show_table(Types & nest)
  {
    for( auto ptr : nest.pos )
    {
      std::print("{} {:8}\n", ptr->name->first, ptr->id);
    }
  }

}