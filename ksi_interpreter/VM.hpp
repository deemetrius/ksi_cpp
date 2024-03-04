#pragma once

#include "infrastructure.hpp"
#include <memory>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::VM
  {
    using config_holder = std::unique_ptr<space_configuration>;

    // props
    config_holder config;
    runtime_info runtime;

    VM()
      : config{ std::make_unique<space_configuration>() }
      , runtime{ config.get() }
    {}

    void run()
    {
      while( ! runtime.call_stack_empty() )
      {
        execution::ptr_instruction instr_handle = runtime.instruction_obtain();
        instr_handle->perform(& runtime);
      }
    }
  };


} // ns
