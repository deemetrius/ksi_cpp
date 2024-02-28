#pragma once

#include "../system_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::execution
  {
    struct position
    {
      index_type
        group_index,
        instr_index;
    };
    struct params_type
    {
      // props
      //fn_action_get
      //fn_action_set
    };

    static void fn_none(ptr_run_info runtime_handle, params_type const & params) {}

    using fn_pointer = decltype(& fn_none);

    struct callable
    {
      // props
      fn_pointer    fn_handle;
      params_type   params;

      // actions

      //void perform(ptr_run_info runtime_handle) const { fn_handle(runtime_handle, params); }
      void operator () (ptr_run_info runtime_handle) const { fn_handle(runtime_handle, params); }
    };

    using group_type = std::vector<callable>;
  };


} // ns
