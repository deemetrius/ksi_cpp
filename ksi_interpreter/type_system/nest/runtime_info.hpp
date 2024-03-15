#pragma once

#include "../runtime/space_data.hpp"
#include "../runtime/stack_values.hpp"
#include "../runtime/call_stack.hpp"

/*
  space_configuration хранит token равный колличеству подгруженных скрипт файлов
  этот token передаётся как параметр конструктора для thread_space (который space_data)

  таким образом можно доподгружать в space_configuration дополнительные скрипт файлы,
  когда уже есть выполняющиеся thread_space's

  later decides how to update thread data from space configuration (token based)
*/

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::run_data
  {
    // props
    runtime::space_data thread_space;
  };


  template <typename Type_config>
  struct system<Type_config>::runtime_info
    : public system<Type_config>::runtime::run_data
    , public system<Type_config>::runtime::stack_values
    , public system<Type_config>::runtime::call_stack
  {};


} // ns
