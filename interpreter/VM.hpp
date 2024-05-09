#pragma once

#include "script_loader.hpp"

namespace ksi::interpreter::execution
{

  struct module_data
  {
    enum class state { not_ready, calculating, ready };

    configuration::module_config                                  * config;
    type_system::clearance::unique<type_system::impl::impl_array> variables = std::make_unique<type_system::impl::impl_array>();
    std::vector<state>                                            variables_statuses;

    module_data(configuration::module_config * h_config) : config{ h_config }
    {
      add_more_variables();
    }

    void add_more_variables()
    {
      std::size_t new_size = config->variables.pos.size();

      variables->elements.reserve(new_size);
      for( std::size_t i = variables->elements.size(); i < new_size; ++i )
      { variables->elements.emplace_back(&variables->point); }

      variables_statuses.resize(new_size, state::not_ready);
    }

    type_system::base::i_value * get_value(std::size_t index, execution::params & params)
    {
      switch( variables_statuses[index] )
      {
        case state::calculating:
        throw state::calculating;

        case state::not_ready: {
          execution::data::call_stack call_stack;
          call_stack.run(config->variables.pos[index]->seq_init.get(), params);
        }
        [[fallthrough]];

        case state::ready:
        break;
      }

      return variables->elements[index].get_value();
    }

    type_system::base::i_value * find_value(sys::literal name, params & p)
    {
      // find constant
      configuration::module_config::read_only_cell * constant = config->constants.find(name);
      if( constant != nullptr ) { return constant->value.keep.handle; }

      // find config variable
      configuration::module_config::variable_information * var_info = config->variables.find(name);
      if( var_info == nullptr ) { return nullptr; }

      if( variables->elements.size() < var_info->position )
      {
        add_more_variables();
      }

      return get_value(var_info->position, p);
    }
  };

  struct thread
  {
    vm_config                 * h_config;
    std::vector<module_data>  modules;
    data::call_stack          call_stack;
    data::stack               stack;

    void add_modules()
    {
      modules.reserve( h_config->modules.pos.size() );
      for( configuration::module_config * module_configuration : h_config->modules.pos )
      {
        modules.emplace_back(module_configuration);
      }
    }

    module_data * get_module(size_t index)
    {
      if( modules.size() < h_config->modules.pos.size() ) { add_modules(); }

      return &modules[index];
    }

    thread(vm_config * h) : h_config{ h }
    {
      add_modules();
    }

    void run(params & p)
    {
      module_data * md = get_module(h_config->module_main->id);
      type_system::base::i_value * value = md->find_value(h_config->literal_do, p);
      configuration::sequence * seq = configuration::sequence::from_value(value, p.vm_configuration->static_information);
      if( seq == nullptr ) { return; }

      call_stack.run(seq, p);
    }
  };

}
namespace ksi::interpreter
{

  struct vm_runtime
  {
    execution::thread one_thread;

    vm_runtime(vm_config * h_config) : one_thread{ h_config }
    {}
  };

  struct VM
  {
    std::unique_ptr<vm_config>  config;
    vm_runtime                  runtime;

    VM() : config{ std::make_unique<vm_config>() }, runtime{ config.get() }
    {}
  };

}
