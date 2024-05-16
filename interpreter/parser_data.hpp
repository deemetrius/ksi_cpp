#pragma one

#include "VM.hpp"
#include "lib/dict_diff.hpp"

namespace ksi::interpreter::loader
{
  struct parser_data
  {
    vm_config                                       * h_configuration;
    lib::dict_diff<sys::string>                     dict{ h_configuration->dict };
    static_table<configuration::module_config> modules;

    configuration::module_config * current_module{ h_configuration->module_main };

    void add_module(sys::string & name)
    {
      sys::literal lit_name = dict.add( std::move(name) );
      current_module = modules.append_row( lit_name ).result;
    }
  };
}