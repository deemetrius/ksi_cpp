#pragma once

#include "../system_nest.hpp"
//#include "ksi_lib/dict.hpp"
#include "ksi_lib/dict_part.hpp"
#include <memory>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::space_configuration
  {
    using dict_type = ksi::lib::dict<t_string>;
    using dict_ptr_type = std::shared_ptr<dict_type>;

    // props
    dict_ptr_type dict;
    ksi::lib::dict_part<t_string> part_test;

    space_configuration()
      : dict{ std::make_shared<dict_type>() }
      , part_test{ dict }
    {
      using namespace std::string_literals;
      std::wstring str{ L"x"s };

      auto show = [](auto result)
      {
        std::wcout << std::boolalpha;
        std::wcout << result.index << "# ";
        if( result.was_added ) { std::wcout << '"' << result.value->term << "\" " << result.value->rank << ' '; }
        std::wcout << result.was_added << '\n';
      };

      show( part_test.has(str) );
      dict->add(str);
      show( part_test.has(str) );
      part_test.add(str + L"1");
      part_test.add(str);
      show( part_test.has(str) );
      show( part_test.has(str + L"1") );
    }
  };


} // ns
