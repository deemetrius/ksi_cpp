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


  template <typename Type_settings>
  struct system<Type_settings>::runtime::thread_space
  {
    // props
    runtime::space_data       space;
    runtime::stack_values     stack;
    runtime::call_stack_info  call_stack;

    void run(ptr_runtime_info runtime_handle)
    {
      while( ! call_stack.chain.empty() )
      {
        execution::ptr_instruction_const instr_handle = call_stack.instruction_obtain();
        instr_handle->perform(runtime_handle, this);
        // we do not call here: call_stack.instruction_next_state();
        // decision ~ it should be called by specific instgructions
      }
    }
  };


} // ns
