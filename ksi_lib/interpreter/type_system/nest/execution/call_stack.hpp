#pragma once

#include "instructions.hpp"
#include <vector>
#include <list>

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::execution::sequence_space
  {
    using instr_positions = std::vector<typename instructions::position>;

    // props
    ptr_sequence sequence_handle;
    //vars
    instr_positions pos_instr_chain;
  };


  template <typename Type_config>
  struct types<Type_config>::execution::call_stack
  {
  private:
    using cs_storage_type = std::list<sequence_space>;

    // props
    cs_storage_type cs_data;

  public:

    // actions
  };


} // ns
