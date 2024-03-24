#pragma once

#include "../nest/space_configuration.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::runtime::space_data
  {
    // props
    ptr_space_configuration   config_handle;
    info::token_type          config_token;

    // ctor
    space_data(ptr_space_configuration space_config_handle)
      : config_handle{ space_config_handle }
      , config_token{ space_config_handle->token }
    {}
  };


} // ns
