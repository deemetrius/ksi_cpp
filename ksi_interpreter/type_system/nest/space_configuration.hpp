#pragma once

#include "../system_nest.hpp"
#include "ksi_lib/dict.hpp"
#include <memory>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::space_configuration
  {
    using dict_type = ksi::lib::dict<t_string>;
    using dict_ptr_type = std::shared_ptr<dict_type>;

    // props
    dict_ptr_type dict;

    space_configuration()
      : dict{ std::make_shared<dict_type>() }
    {}
  };


} // ns
