#pragma once

#include "parser_rules.hpp"

namespace ksi::interpreter::loader
{
  using namespace std::string_literals;

  struct parser
  {
    vm_config * h_config;

    void parse(sys::string const & file)
    {
      state st{
        .pos{ file.data(), file.data() + file.size() },
        .prepare{ .h_configuration = h_config },
        .fn = &rules::scope_file::function
      };

      do
      {
        if( std::isspace(*st.pos.current) ) { ++st.pos.current; continue; }
        st.fn(st);
        if( st.pos.is_end() && (st.is_done == false) )
        {
          st.message = "Token not the last one"s;
        }
      }
      while( (st.is_done == false) && st.message.empty() );

      if( st.message.size() )
      {
        std::print("{}\n", st.message); exit(1);
      }

      lib::show_vector( st.prepare.seq_current_group().list_instructions );
    }
  };

}
