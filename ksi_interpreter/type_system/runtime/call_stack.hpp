#pragma once

#include "../info/sequence.hpp"
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
  struct system<Type_config>::runtime::call_stack_info
  {
  private:
    using call_stack_storage = std::list<sequence_space>;

    // props
    call_stack_storage chain;

  public:

    // actions

    bool call_stack_empty() const
    {
      return chain.empty();
    }

    void call_stack_pop()
    {
      if( chain.size() < 1 ) { return; }
      if(
        ptr_sequence_space seq_space_handle{ & chain.back() };
        seq_space_handle->pos_instr_chain.size() > 1
      ) {
        seq_space_handle->pos_instr_chain.pop_back();
      }
      else
      {
        chain.pop_back();
      }
    }

    execution::ptr_instruction_const instruction_obtain() const
    {
      ptr_sequence_space seq_space_handle{ & chain.back() };
      execution::position pos = seq_space_handle->pos_instr_chain.back();
      return seq_space_handle->sequence_handle->instruction_get_pointer(pos);
    }

    void instruction_next_state()
    {
      if( chain.empty() ) { return; }

      {
        ptr_sequence_space seq_space_handle{ & chain.back() };
        ptr_sequence seq_handle{ seq_space_handle->sequence_handle };
        typename execution::ptr_position pos_handle{ & seq_space_handle->pos_instr_chain.back() };
        if( execution::try_increment_position(pos_handle, seq_handle->instruction_groups) )
        {
          return;
        }
      }

      call_stack_pop();
    }
  };


} // ns
