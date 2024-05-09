
#include <print>
#include "interpreter/VM.hpp"
#include "interpreter/script_loader.hpp"

template <typename Types>
void show_types(Types & nest)
{
  for( auto ptr : nest.pos )
  {
    std::print("{:3}  {}\n", ptr->id, ptr->name->first);
  }
}

int main()
{
  namespace ts = ksi::interpreter::type_system;
  namespace rng = ksi::interpreter;
  using namespace std::literals::string_literals;

  try
  {

    {
      ksi::interpreter::VM vm;

      std::print("categories:\n");
      show_types(vm.config->static_information.r.category_table);
      std::print("\nsystem types:\n");
      show_types(vm.config->static_information.r.type_table);

      std::print("size: {}\n", vm.runtime.one_thread.get_module(vm.config->module_main->id)->variables->elements.size() );

      rng::loader::script_parser parser{ vm.config.get() };
      parser.parse("\n @main "s);

      // ts::brick::flag_join = true;
      // ts::brick::flag_point = true;
    }

    ksi::lib::exception::rethrow();
  }
  catch( std::bad_alloc const & e )
  {
    std::print("* Out of memory exception");
  }
  catch( ksi::lib::message const & message )
  {
    std::print("* Exception: {}\n", message);
  }
  catch( ... )
  {
    std::print("* Exception!\n");
  }

  std::print("cnt_array: +{} -{}\n", ts::collections::array::cnt_made, ts::collections::array::cnt_out);
  std::print("cnt_cells: +{} -{}\n", ts::brick::cell_type::cnt_made, ts::brick::cell_type::cnt_out);
  std::print("Done {}\n", ksi::lib::exception::skip_count);
}
