
#include <iostream>

#if 0
//#include "../ksi_lib/conv.integer.hpp"
//#include "../ksi_lib/conv.string.hpp"
#include "../ksi_lib/pattern.hpp"

int main()
{
  using regex_nest = ksi::lib::regex_nest<std::string>;
  regex_nest::pattern pattern{ regex_nest::pattern::prefix(".txt", "i") };
  std::cout << pattern.value << " [ " << pattern.regex << " ]\n";
  std::cout << "doc.txt match: " << pattern.match("doc.txt") << "\n";
  std::cout << "doc.txt replace ext: " << pattern.replace("doc.txt", ".hpg") << "\n";
  return 0;
}
#endif

#include "../ksi_lib/dict.hpp"

int main()
{
  using dict_type = ksi::lib::dict<std::string>;
  dict_type dict;
  dict.try_add("enum");
  dict.try_add("struct");
  dict.try_add("action");
  for( typename dict_type::reference item : dict.get_range() )
  {
    std::cout
    << "name: " << item.name << "\n"
    << "key: " << item.key << "\n"
    << "index: " << item.index << "\n"
    << "\n";
  }
  return 0;
}
