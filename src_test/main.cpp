
#include <iostream>


#if 1
#include "ksi_lib/interpreter/type_system/infrastructure.hpp"

int main()
{
  ksi::interpreter::system<>::system_types sys_types;
  ksi::interpreter::system<>::values::value_bool v_bool{ true };
  ksi::interpreter::system<>::values::value_array v_array{ 3 };

  std::wcout << v_bool.get_type(&sys_types)->name << L'\n';

  ksi::interpreter::system<>::run_info runtime;

  return 0;
}
#endif


#if 0
#include "ksi_lib/pattern.hpp"

int main()
{
  using regex_nest = ksi::lib::regex_nest<std::string>;
  using namespace std::string_literals;

  std::string text = "doc.TXT"s;

  {
    regex_nest::maybe_pattern maybe = regex_nest::maybe_pattern::make_regular_try("\\w+$"s, ""s);
    regex_nest::pattern pattern = regex_nest::pattern::make_ending(".txt"s, "i"s);
    std::cout << "pattern: " << pattern.source_string << " [ " << pattern.pattern_string << " ] " << pattern.mode_chars << "\n";

    std::cout << text << " match: " << std::boolalpha << pattern.match("doc.tXt") << "\n";
    std::cout << text << " replaced extension: " << pattern.replace("doc.txt", ".hpg") << "\n";
  }

  {
    regex_nest::pattern pattern = regex_nest::pattern::make_regular("\\.t(x)t$"s, "i"s);
    std::cout << "\npattern: " << pattern.pattern_string << " find_first() in: " << text << '\n';
    for( regex_nest::size_type i{ 0 }; regex_nest::match_range const & it : pattern.find_first(text) )
    {
      std::cout << "match[" << i << "]: " << it.make_view(text) << '\n';
      it.find_next(pattern.regex, text);
      ++i;
    }
  }

  {
    regex_nest::pattern pattern = regex_nest::pattern::make_regular(R"(^\W*(\d+)\.txt$)"s, "i"s);
    regex_nest::string_type text{ "\t1.txt\n\t2.txt\n\t3.txt"s };

    std::cout << "\ntext:\n" << text << "\npattern: " << pattern.pattern_string << '\n';

    regex_nest::search_result all_results = pattern.full_search(text);
    for( regex_nest::size_type ie{ 1 }; regex_nest::find_result const & entry : all_results )
    {
      std::cout << "entry " << ie << "\n";
      for( regex_nest::size_type ic{ 0 }; regex_nest::match_range capture : entry )
      {
        std::cout << "match[" << ic << "]: " << capture.make_view(text) << '\n';
        ++ic;
      }
      ++ie;
    }
  }

  return 0;
}
#endif


#if 0
#include "ksi_lib/interpreter/var_names.hpp"

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
