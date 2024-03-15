#pragma once

#include "infrastructure.hpp"
#include <memory>

/*
  VM
    config
    runtime
      threads

  для начала сделать single thread
*/

// парсер формирует patch структуру (из скрипт файлов)
// если ошибок нет, то patch можно применить в VM

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
        execution::ptr_instruction_const instr_handle = runtime.instruction_obtain();
        instr_handle->perform(& runtime);
        // we do not call here: runtime.instruction_next_state();
        // decision ~ it should be called by specific instgructions
      }
    }
  };


} // ns
