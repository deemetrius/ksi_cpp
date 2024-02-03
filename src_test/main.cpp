
#include <iostream>

#if 1
//#include "../ksi_lib/conv.integer.hpp"
//#include "../ksi_lib/conv.string.hpp"
#include "../ksi_lib/pattern.hpp"

int main()
{
  using regex_nest = ksi::lib::regex_nest<std::string>;
  using namespace std::string_literals;

  std::string text = "doc.TXT"s;

  {
    regex_nest::maybe_pattern maybe = regex_nest::maybe_pattern::regular_try("\\w+$"s, ""s);
    regex_nest::pattern pattern = regex_nest::pattern::ending(".txt"s, "i"s);
    std::cout << "pattern: " << pattern.source_string << " [ " << pattern.pattern_string << " ] " << pattern.mode_chars << "\n";

    std::cout << text << " match: " << std::boolalpha << pattern.match("doc.tXt") << "\n";
    std::cout << text << " replaced extension: " << pattern.replace("doc.txt", ".hpg") << "\n";
  }

  {
    regex_nest::pattern pattern = regex_nest::pattern::regular("\\.t(x)t$"s, "i"s);
    //typename regex_nest::find_result matches = pattern.find_first(text, 0);
    std::cout << "\npattern: " << pattern.pattern_string << " find_first() in: " << text << '\n';
    for( regex_nest::size_type i{ 0 }; regex_nest::match_range const & it : pattern.find_first(text) )
    {
      std::cout << "match[" << i << "]: " << it.make_view(text) << '\n';
      it.find_next(pattern.regex, text);
      ++i;
    }
  }

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
