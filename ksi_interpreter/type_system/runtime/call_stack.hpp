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
    ptr_sequence sequence_handle;
    //vars
    instr_positions pos_instr_chain;
  };


  template <typename Type_config>
  struct system<Type_config>::runtime::call_stack
  {
  private:
    using cs_storage_type = std::list<sequence_space>;

    // props
    cs_storage_type cs_data;

  public:

    // actions
  };


} // ns
