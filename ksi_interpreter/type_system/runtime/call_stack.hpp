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
    //using cs_storage_iterator = typename cs_storage_type::iterator;

    // props
    call_stack_storage call_stack_data;

  public:

    // actions

    bool call_stack_empty() const { return cs_data.empty(); }

    //cs_storage_iterator sequence_top() { return cs_data.end(); }

    execution::ptr_instruction instruction_obtain() const
    {
      ptr_sequence_space seq_space_handle{ & cs_data.back() };
      execution::position pos = seq_space_handle->pos_instr_chain.back();
      return &(
        seq_space_handle->sequence_handle->instruction_groups[pos.group_index][pos.instr_index]
      );
    }
  };


} // ns
