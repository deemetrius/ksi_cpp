
#if 0
#include "ksi_ui/ui.hpp"

int main()
{
  ui::theme theme{ ui::themes::first_theme(), {.theme_name = "Blue"} };

  ui::control_pointer btn = ui::make<ui::controls::Button>({.caption = "click me"});
  ui::control_pointer lbl = ui::make<ui::controls::Label>({.text = "Hello", .for_control = btn});

  theme.draw(btn);
  theme.draw(lbl);
}
#endif


#if 1
#include <iostream>
#include "ksi_interpreter/infrastructure.hpp"
#include <ranges>

#include "ksi_lib/table.hpp"

void show_dict(auto const & dict)
{
  std::cout << '\n';
  for( auto const & it : dict.set )
  {
    std::wcout << it.name << " \t~ " << it.rank << "\tid: " << it.id << '\n';
  }
}

int main()
{
  using sys = ksi::interpreter::system<>;
  try
  {
    sys::values::value_bool v_bool{ true };
    sys::values::value_array v_array{ 3 };

    std::cout << std::boolalpha;

    std::cout
    << "direct: "
    << sys::configuration::table_of_modules::auto_increment_direct
    << "\n";
    std::cout
    << "need_cast: "
    << sys::configuration::table_of_modules::auto_increment_need_cast
    << "\n";

    sys::VM vm;

    show_dict(*vm.config->dict);
    std::wcout << v_bool.get_type(&vm.config->sys_types)->name->name << L"\n\n";

    sys::patch_vm patch_vm{ vm.config->dict };
    std::cout << "patch.has(ret): " << patch_vm.dict.has(L"ret").included() << '\n';
    std::cout << "patch.add(ret): " << patch_vm.dict.add(L"ret").was_added << '\n';
    std::cout << "patch.add(z1): " << patch_vm.dict.add(L"z1").was_added << '\n';
    std::cout << "patch.add(z0): " << patch_vm.dict.add(L"z0").was_added << '\n';

    show_dict(patch_vm.dict.extra);

    std::cout << "\napply patch\n";
    patch_vm.dict.apply();
    show_dict(*vm.config->dict);

    sys::info::literal_type mod_name_main = vm.config->dict->add(L"@main").it->get_const();
    vm.config->modules.emplace_back( std::in_place_type<sys::info::meta_info>, mod_name_main );

    sys::info::literal_type mod_name = vm.config->dict->add(L"@global").it->get_const();
    vm.config->modules.emplace_back( std::in_place_type<sys::info::meta_info>, mod_name );

    for( auto [key, val] : vm.config->modules.index )
    {
      std::wcout << key->name << L" ~ " << val->position << L"\n";
    }
    std::wcout << vm.config->modules.find(mod_name_main)->name->name << L"\n";
  }
  catch( ... )
  {
    std::cout << "exception\n";
  }

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
