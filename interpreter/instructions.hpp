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

}