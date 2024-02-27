#pragma once

#include "../types_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::instructions
  {
    struct position
    {
      index_type
        group_index,
        instr_index;
    };
    struct params_type {};

    static void fn_none(ptr_run_info runtime_handle, params_type const & params) {}

    using fn_pointer = decltype(& fn_none);

    struct instruction
    {
      // props
      fn_pointer    fn_handle;
      params_type   params;

      // actions

      //void perform(ptr_run_info runtime_handle) const { fn_handle(runtime_handle, params); }
      void operator () (ptr_run_info runtime_handle) const { fn_handle(runtime_handle, params); }
    };

    using group_type = std::vector<instruction>;
  };


} // ns
