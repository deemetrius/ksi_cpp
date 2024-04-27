#pragma once

  #include <cstdio>
  #include "i_log.hpp"

namespace ksi::lib::auxiliary {


  template <typename Location, typename Data, typename Writer>
  struct log_console :
    i_log<Location, Data>
  {
    void append(Data * record, Location from) override
    {
      Writer::show_location(stdout, from);
      Writer::write_message(stdout, *record);
    }

    using base_type = i_log<Location, Data>;
  };


}
