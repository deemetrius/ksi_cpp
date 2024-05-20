#pragma one

#include "VM.hpp"
#include "lib/dict_diff.hpp"

namespace ksi::interpreter::loader
{

  struct position
  {
    using type = const char *;

    type current, end;

    bool is_end() const { return (current == end); }
  };

  struct prepare_type
  {
    enum class prev_cell_type { none, constant, variable };

    vm_config                                  * h_configuration;
    lib::dict_diff<sys::string>                dict{ h_configuration->settings.dict };
    static_table<configuration::module_config, meta_info> modules;

    sys::string name;
    std::size_t line_number;
    prev_cell_type was_cell = prev_cell_type::none;
    std::list< sys::unique<execution::sequence> > seq_list;
    configuration::module_config * current_module = h_configuration->settings.module_main;

    execution::group_instructions & seq_current_group()
    {
      if( seq_list.empty() )
      {
        sys::unique<execution::sequence> seq = std::make_unique<execution::sequence>();
        seq_list.push_front( std::move(seq) );
      }

      if( seq_list.front()->groups.empty() )
      {
        seq_list.front()->groups.emplace_back();
      }

      return seq_list.front()->groups.front();
    }

    void next_cell()
    {
      switch( this->was_cell )
      {
        case prev_cell_type::constant :
        add_constant();
        break;

        default: ;
      }
    }

    void add_constant()
    {
      sys::literal lit_name = dict.add( std::move(this->name) );
      current_module->constants.append_row( lit_name, std::move(this->seq_list.front()), this->line_number );
      this->seq_list.pop_front();
      this->was_cell = prev_cell_type::none;
    }
  };

  struct state
  {
    enum class fn_result { keep_continue, error_occured, fine_exit };
    static fn_result sample_function(state & st) { return fn_result::fine_exit; }
    using function_type = decltype(&sample_function);

    // props

    std::size_t line_number = 0;
    position    pos;

    prepare_type          prepare;

    void add_module(parser_result & src)
    {
      sys::literal lit_name = prepare.dict.add( std::move(src.name) );
      prepare.current_module = prepare.modules.append_row( src.line, lit_name ).result;
    }

    function_type fn;
    sys::string   message;

    void finish()
    {
      if( prepare.seq_list.size() == 1 )
      {
        prepare.next_cell();
      }
    }
  };

}