#pragma once

#include "parser_rules.hpp"

namespace ksi::interpreter::loader
{

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

      state::fn_result status = state::fn_result::keep_continue;
      do
      {
        // keep_continue error_occured fine_exit
        if( std::isspace(*st.pos.current) ) { ++st.pos.current; continue; }
        switch( status = st.fn(st) )
        {
          case state::fn_result::keep_continue : continue;
          case state::fn_result::error_occured :
          {
            std::print("{}\n", st.message);
            return;
          }
          default: ;
        }
      }
      while( status != state::fn_result::fine_exit );

      st.finish();
      std::print(":parsed\n");

      lib::show_table(st.prepare.current_module->constants);
    }
  };

}
