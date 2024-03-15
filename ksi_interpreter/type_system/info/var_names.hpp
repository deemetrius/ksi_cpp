#pragma once

#include "../system_nest.hpp"
#include "ksi_lib/dict_part.hpp"
#include <vector>

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::info::var_names
  {
    using map_type = ksi::lib::dict_part<t_string>;
    using dict_iterator = map_type::dict_iterator;
    using result_type = map_type::result_type;
    using vector_type = std::vector<dict_iterator>;

    // props
    map_type      prop_map;
    vector_type   prop_names;

    // actions

    result_type add(t_string name)
    {
      result_type ret{ prop_map.add( std::move(name) ) };
      if( ret.was_added ) { prop_names.push_back(value); }
      return ret;
    }
  };


} // ns
