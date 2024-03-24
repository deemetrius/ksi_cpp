#pragma once

#include "../runtime/thread_space.hpp"

/*
  space_configuration хранит token равный колличеству подгруженных скрипт файлов
  этот token передаётся как параметр конструктора для thread_space

  таким образом можно доподгружать в space_configuration дополнительные скрипт файлы,
  когда уже есть выполняющиеся thread_space's

  later decides how to update thread data from space configuration (token based)
*/

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::runtime_info
  {
    runtime::thread_space first_page;
  };


} // ns
