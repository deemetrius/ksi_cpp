#pragma once

#include "../values/value_array.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::stack_values
  {
    using stack_frames_type = std::vector<stack_frame_holder>;

    // props
    stack_frames_type stack_frames;

    ptr_stack_frame stack_frame_current()
    {
      return & stack_frames.back();
    }
  };


} // ns
