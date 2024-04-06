#pragma once

  #include "../values/system_types.hpp"
  #include "configuration.hpp"

/*
  тут карта модулей
  это информация для формирования runtime::space_data
  но сами сиквенсы живут тут (именно в записях карточки модуля)
*/

/*
  есть load_time, а есть run_time
  когда появится желание менять sequence прямо в run_time,
  то сиквенсы видимо придётся поместить и в другую область
*/

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::space_configuration
  {
    // props
    log::internal_log_holder          log;
    info::dict_holder_type            dict;
    info::token_type                  token{ 0 };
    values::system_types              sys_types;
    configuration::table_of_modules   modules;

    space_configuration(log::internal_log_holder log_keep)
      : log{ (log_keep == nullptr) ? std::make_shared<typename log::internal_logger_none>() : std::move(log_keep) }
      , dict{ std::make_shared<typename info::dict_type>() }
      , sys_types{ dict.get(), log.get() }
    {}
  };


} // ns
