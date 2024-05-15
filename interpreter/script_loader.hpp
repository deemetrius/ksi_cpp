#pragma once

#include "parser_rules.hpp"

namespace ksi::interpreter::loader
{

  struct parser
  {
    vm_config * h_config;

    void parse(sys::string const & file)
    {
      rules::state st{
        .pos{ file.data(), file.data() + file.size() },
        .data{ h_config },
        .fn = &rules::scope_file::function
      };

      do
      {
        st.fn(st);
      }
      while( st.message.empty() );

      if( st.message.size() )
      {
        std::print("{} ", st.message); exit(1);
      }

      lib::show_table(st.data.modules);
    }
  };

}
