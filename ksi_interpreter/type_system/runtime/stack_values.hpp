#pragma once

#include "../values/value_array.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::runtime::stack_values
  {
    using stack_frames_type = std::vector<stack_frame_holder>;

    // props
    stack_frames_type frames;

    ptr_stack_frame stack_frame_current()
    {
      return & frames.back();
    }
  };


} // ns
