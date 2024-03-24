#pragma once

#include "space_configuration.hpp"
#include "runtime_info.hpp"
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


  template <typename Type_settings>
  struct system<Type_settings>::VM
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
      runtime.first_page.run(this);
    }
  };


} // ns
