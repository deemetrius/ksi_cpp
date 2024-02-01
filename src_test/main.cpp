
#include <iostream>

#if 1
//#include "../ksi_lib/conv.integer.hpp"
//#include "../ksi_lib/conv.string.hpp"
#include "../ksi_lib/pattern.hpp"

int main()
{
  using regex_nest = ksi::lib::regex_nest<std::string>;

  regex_nest::maybe_pattern maybe = regex_nest::maybe_pattern::regular_try("\\w+$", "");
  regex_nest::pattern pattern{ regex_nest::pattern::ending(".txt", "i") };
  std::cout << "pattern: " << pattern.source_string << " [ " << pattern.pattern_string << " ] " << pattern.mode_chars << "\n";

  std::string text = "doc.tXt";

  std::cout << text << " match: " << std::boolalpha << pattern.match("doc.tXt") << "\n";
  std::cout << text << " replaced extension: " << pattern.replace("doc.txt", ".hpg") << "\n";
  return 0;
}
#endif

#if 0
#include "../ksi_lib/interpreter/vars.hpp"

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
    << "index: " << item.index << "\n"
    << "value: " << item.value << "\n"
    << "\n";
  }
  return 0;
}
#endif
