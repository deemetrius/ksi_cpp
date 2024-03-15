#pragma once

#include "../system_nest.hpp"
#include "ksi_lib/dict.hpp"
#include <memory>

/*
  тут карта модулей
  это информация для формирования runtime::space_data
  но сами сиквенсы живут тут (именно в записях карточки модуля)
*/

/*
  есть load_time, а есть run_time
  когда появится желание менять sequence прямо в run_time,
  то сиквенсы видимо придётся переместить в другую область
*/

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
