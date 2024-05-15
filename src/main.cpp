
#include "lib/table_show.hpp"
#include "interpreter/VM.hpp"
#include "interpreter/script_loader.hpp"

#include <cstdio>
#include <iostream>


std::size_t file_size(std::FILE * fp)
{
  std::fseek(fp, 0, SEEK_END);
  auto ret = std::ftell(fp);
  std::rewind(fp);
  return ret;
}

std::string read_file(std::string const & path)
{
  std::FILE * fp = std::fopen(path.c_str(), "r");
  if( fp == nullptr ) { ksi::lib::msg("File not found"); }

  std::string ret;
  ret.assign(file_size(fp), '-');

  ret.resize(
    std::fread(ret.data(), 1, ret.size(), fp)
  );

  std::fclose(fp);
  return ret;
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

      /* std::print("categories:\n");
      show_table(vm.config->static_information.r.category_table);
      std::print("\nsystem types:\n");
      show_table(vm.config->static_information.r.type_table);

      std::print("size: {}\n", vm.runtime.one_thread.get_module(vm.config->module_main->id)->variables->elements.size() );

      rng::loader::script_parser parser{ vm.config.get() };
      parser.parse("\n @main "s); */

      std::string body = read_file(".out/test.txt");
      std::print("{}\n--\n\n", body);

      ksi::interpreter::loader::parser ps{ vm.config.get() };
      ps.parse(body);

      std::cout << "\n:parsed\n";

      // ts::brick::flag_join = true;
      // ts::brick::flag_point = true;
    }

    ksi::lib::exception::try_rethrow();
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

  // std::print("cnt_array: +{} -{}\n", ts::collections::array::cnt_made, ts::collections::array::cnt_out);
  // std::print("cnt_cells: +{} -{}\n", ts::brick::cell_type::cnt_made, ts::brick::cell_type::cnt_out);
  // ksi::lib::exception::skip_count
}
