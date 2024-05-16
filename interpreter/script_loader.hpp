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
        if( std::isspace(*st.pos.current) ) { ++st.pos.current; continue; }
        st.fn(st);
        st.is_done = st.pos.is_end();
      }
      while( (st.is_done == false) && st.message.empty() );

      if( st.message.size() )
      {
        std::print("{}\n", st.prepare.name); exit(1);
      }

      lib::show_table(st.data.modules);
    }
  };

}
