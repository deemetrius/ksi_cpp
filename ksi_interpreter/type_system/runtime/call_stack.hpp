#pragma once

#include "../nest/execution.hpp"
#include <vector>
#include <list>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::sequence_space
  {
    using instr_positions = std::vector<typename execution::position>;

    // props
    ptr_sequence        sequence_handle;
    stack_frame_holder  variables;
    stack_frame         constants;
    instr_positions     pos_instr_chain;
  };


  template <typename Type_config>
  struct system<Type_config>::runtime::call_stack
  {
  private:
    using call_stack_storage = std::list<sequence_space>;

    // props
    call_stack_storage call_stack_data;

  public:

    // actions

    bool call_stack_empty() const
    {
      return call_stack_data.empty();
    }

    void call_stack_pop()
    {
      if( call_stack_data.size() < 1 ) { return; }
      if(
        ptr_sequence_space seq_space_handle{ & call_stack_data.back() };
        seq_space_handle->pos_instr_chain.size() > 1
      ) {
        seq_space_handle->pos_instr_chain.pop_back();
      }
      else
      {
        call_stack_data.pop_back();
      }
    }

    execution::ptr_instruction_const instruction_obtain() const
    {
      ptr_sequence_space seq_space_handle{ & call_stack_data.back() };
      execution::position pos = seq_space_handle->pos_instr_chain.back();
      return seq_space_handle->sequence_handle->instruction_get_pointer(pos);
    }

    void instruction_next_state()
    {
      //
      {
        ptr_sequence_space seq_space_handle{ & call_stack_data.back() };
        ptr_sequence seq_handle{ seq_space_handle->sequence_handle };
        execution::ptr_position pos_handle { & seq_space_handle->pos_instr_chain.back() };
        if( pos_handle->increment() ) { return; }
      }
      call_stack_pop();
    }
  };


} // ns
