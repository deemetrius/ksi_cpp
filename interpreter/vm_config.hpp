#pragma once

#include "type_system.hpp"
#include "execution.hpp"

namespace ksi::interpreter
{

  struct vm_config
  {
    std::shared_ptr<sys::dictionary>                dict = std::make_shared<sys::dictionary>();
    type_system::info::static_data                  static_information{ dict.get() };
    sys::static_table<configuration::module_config> modules;

    configuration::module_config * module_main;

    sys::literal literal_main_module{ dict->add(sys::string{"@main"sv}).pointer };
    sys::literal literal_do{ dict->add(sys::string{"do"sv}).pointer };

    vm_config()
    {
      module_main = modules.append_row<type_system::meta::meta_information>( literal_main_module );
    }
  };

}
