#pragma once

#include <print>

namespace ksi::lib
{

  template <typename Types>
  void show_table(Types & nest)
  {
    std::print("\n_[ name ]_______[ id ]\n");
    for( auto ptr : nest.pos )
    {
      std::print("  {}\t\t| {:02} |\n", ptr->name->first, ptr->id);
    }
    std::print("\n");
  }

}