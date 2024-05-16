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
    lib::dict_diff<sys::string>                dict{ h_configuration->dict };
    static_table<configuration::module_config, meta_information> modules;

    sys::string name;
    prev_cell_type was_cell = prev_cell_type::none;
    std::list< sys::unique<execution::sequence> > seq;
    //sys::unique<execution::sequence> seq = std::make_unique<execution::sequence>();
    configuration::module_config * current_module = h_configuration->module_main;

    execution::group_instructions & seq_current_group()
    {
      if( seq.front()->groups.empty() )
      {
        seq.front()->groups.emplace_back();
      }

      return seq.front()->groups.front();
    }

    void next_cell()
    {
      switch( this->was_cell )
      {
        case prev_cell_type::constant :
        try_add_constant();
        break;

        default: ;
      }
    }

    void try_add_constant()
    {
      if( ! name.empty() )
      {
        sys::literal lit_name = dict.add( std::move(this->name) );
        current_module->constants.append_row( lit_name, std::move(this->seq.front()) );
        this->seq.pop_front();
      }
    }
  };

  struct state
  {
    static void sample_function(state & st) {}
    using function_type = decltype(&sample_function);

    // props

    std::size_t line_number = 0;
    position    pos;

    prepare_type          prepare;

    void add_module(sys::string & name)
    {
      sys::literal lit_name = prepare.dict.add( std::move(name) );
      prepare.current_module = prepare.modules.append_row( lit_name ).result;
    }

    function_type fn;
    sys::string   message;
    bool          is_done = false;
  };

}