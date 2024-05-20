#pragma once

#include "execution.hpp"
#include "instructions.hpp"

namespace ksi::interpreter
{

  struct vm_runtime
  {
    execution::thread one_thread;

    vm_runtime(vm_config_data * h_config) : one_thread{ h_config }
    {}
  };

  struct VM
  {
    vm_config_data  config;
    vm_runtime      runtime;

    VM() : runtime{ &config }
    {}
  };

}
