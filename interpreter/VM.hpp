#pragma once

#include "execution.hpp"
#include "instructions.hpp"
#include "script_loader.hpp"

namespace ksi::interpreter
{

  struct vm_runtime
  {
    execution::thread one_thread;

    vm_runtime(vm_config * h_config) : one_thread{ h_config }
    {}
  };

  struct VM
  {
    std::unique_ptr<vm_config>  config;
    vm_runtime                  runtime;

    VM() : config{ std::make_unique<vm_config>() }, runtime{ config.get() }
    {}
  };

}
