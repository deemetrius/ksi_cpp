#pragma once

#include "../system_nest.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::execution
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

    static void fn_none(ptr_runtime_info runtime_handle, ptr_thread_space page_handle, params_type const & params) {}

    using fn_pointer = decltype(& fn_none);

    struct callable
    {
      // props
      fn_pointer    fn_handle;
      params_type   params;

      // actions

      void perform(ptr_runtime_info runtime_handle, ptr_thread_space page_handle) const
      {
        fn_handle(runtime_handle, page_handle, params);
      }
    };

    using group_type = std::vector<callable>;
    using isntr_groups = std::vector<group_type>;
    using ptr_instruction_const = callable const *;
    using ptr_position = position *;

    static bool try_increment_position(ptr_position pos_handle, isntr_groups const & groups)
    {
      return (
        ( (pos_handle->instr_index + 1) < groups[pos_handle->group_index].size() ) ?
        ( ++pos_handle->instr_index, true ) :
        false
      );
    }
  };


} // ns
