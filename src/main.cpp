
#include "interpreter/system.hpp"

template <typename Types>
void show_types(Types & nest)
{
  for( auto ptr : nest.pos )
  {
    std::print("{} {:8} \t{}\n", ptr->type_index, ptr->name->first, ptr->position);
  }
}

int main()
{
  ksi::interpreter::log_system::to_console con;
  con.append(
    & ksi::interpreter::log_messages::startup_messages::intro,
    std::source_location::current()
  );

  std::print("{{---* hello *---}}\n");

  ksi::interpreter::system::system_types::system_data sys_data;
  show_types(sys_data.p.types_nest);

  return 0;
}
