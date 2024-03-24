#pragma once

#include "../system_nest.hpp"
#include "ksi_lib/dict_patch.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::patch_vm
  {
    // props
    ksi::lib::dict_patch<t_string> dict;
  };


} // ns
