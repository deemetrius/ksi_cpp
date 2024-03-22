#pragma once

#include <string>
#include "ksi_lib/dict.hpp"
#include <utility>

namespace ui {


  struct system
  {
    using string_type = std::string;
    using dict_type = ksi::lib::dict<string_type>;
    using control_type = dict_type::iterator;

    static inline dict_type control_types{};

    static control_type reg_control_type(string_type type_name)
    {
      /*
        it's ok to register same type_name twice
        'coz same control_type will be returned in this case
      */

      // if you assign same control_type to different classes then plz no blame me //

      /*
        // otherwise here can be something like this ~
        typename dict_type::result_add res = control_types.add( std::move(type_name) );
        if( res.already_exists() ) { throw some_thing; }
        return res.it;
      */

      return system::control_types.add( std::move(type_name) ).it;
    }
  };


} // ns
