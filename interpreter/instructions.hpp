#pragma once

namespace ksi::interpreter::instructions
{
  using namespace std::literals::string_view_literals;

  template <typename Type>
  execution::instruction_with_function literal_to_stack{ "literal_to_stack"sv,
    [](execution::params & params, execution::data_type const & data)
    {
      bool v = data.template get<Type>();
      params.h_tread->stack.frames.front()->append( type_system::creation::make_bool(v) );
    }
  };

  execution::instruction_with_function module_constant_to_stack{
    "module_constant_to_stack",
    [](execution::params & params, execution::data_type const & data)
    {
      configuration::module_config::read_only_cell * h_cell{
        params.config->data->settings.modules.pos[data.module_cell_position.module_id]->constants.pos[data.module_cell_position.constant_id]
      };
      switch( h_cell->status )
      {
        case configuration::module_config::cell_status::not_initialized : {
          //std::lock_guard<std::mutex> lock{ params.config-> };
          h_cell->status = configuration::module_config::cell_status::calculating;
          execution::thread thread{ params.config };
          execution::params p{
            params.config,
            &thread
          };
          thread.call_stack.run(h_cell->seq_init.get(), p);

          h_cell->status = configuration::module_config::cell_status::ready;
        }
        break;

        case configuration::module_config::cell_status::calculating :
        throw "Not yet initialized"sv;
        break;

        case configuration::module_config::cell_status::ready :
        break;
      }
    }
  };

}