
//#include "../ksi_lib/conv.integer.hpp"
//#include "../ksi_lib/conv.string.hpp"
#include "../ksi_lib/pattern.hpp"
#include <iostream>

int main()
{
  using regex_nest = ksi::lib::regex_nest<std::string>;
  regex_nest::pattern pattern{ regex_nest::pattern::ending(".txt", "i") };
  std::cout << pattern.value << " [ " << pattern.regex << " ]\n";
  std::cout << "doc.txt match: " << pattern.match("doc.txt") << "\n";
  std::cout << "doc.txt replace ext: " << pattern.replace("doc.txt", ".hpg") << "\n";
  return 0;
}
