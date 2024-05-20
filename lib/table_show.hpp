#pragma once

#include <print>

namespace ksi::lib
{

  template <typename Types>
  void show_table(Types & nest)
  {
    std::print("\n_[ name ]_________[ id ]\n");
    for( auto ptr : nest.pos )
    {
      std::print("  {:16}| {:02} |\n", ptr->name->first, ptr->id);
    }
    std::print("\n");
  }

}