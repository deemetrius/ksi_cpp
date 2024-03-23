#pragma once

#include "../system_nest.hpp"

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
    // props
    info::dict_ptr_type   dict;
    info::token_type      token{ 0 };

    space_configuration()
      : dict{ std::make_shared<typename info::dict_type>() }
    {}
  };


} // ns
