#pragma once

namespace ksi::lib::auxiliary
{


  template <typename Location, typename Data>
  struct i_log
  {
    virtual void append(Data * record, Location from) = 0;
  };


}