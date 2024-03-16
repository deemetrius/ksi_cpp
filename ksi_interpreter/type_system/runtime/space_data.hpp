#pragma once

#include "../nest/space_configuration.hpp"
#include "system_types.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::runtime::space_data
  {
    // props
    ptr_space_configuration           config_handle;
    space_configuration::token_type   config_token;
    system_types                      sys_types;

    // ctor
    space_data(ptr_space_configuration space_config_handle)
      : config_handle{ space_config_handle }
      , config_token{ space_config_handle->token }
    {}
  };


} // ns
