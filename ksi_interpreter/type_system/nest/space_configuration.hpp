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
      std::wstring str{ L"1"s };

      std::wcout << part_test.has(str).index << "# " << '\n';
      dict->add(str);
      std::wcout << part_test.has(str).index << "# " << part_test.has(str).value->term << '\n';
    }
  };


} // ns
