#pragma once

  #include <string_view>

namespace ksi::lib {


  using dict_id_type = std::size_t;
  using dict_rank_type = std::size_t;


} // ns
namespace ksi::lib::errors::dict_params {


  struct vector_only
  {
    dict_id_type  vector_id;

    void throw_exception();
  };

  struct map_mismatch
  {
    dict_id_type  vector_id, map_id;

    void throw_exception();
  };


} // ns
namespace ksi::lib::errors {


  struct base
  {
    std::string_view msg;
  };


  struct dict_inconsistent {};

  struct dict_inconsistent_vector_only
    : public dict_params::vector_only
    , public base
    , public dict_inconsistent
  {};

  struct dict_inconsistent_map_mismatch
    : public dict_params::map_mismatch
    , public base
    , public dict_inconsistent
  {};


} // ns
namespace ksi::lib::errors::dict_params {
  using namespace std::string_view_literals;


  void vector_only::throw_exception()
  {
    throw dict_inconsistent_vector_only{*this, "dict::add() ~ Unable to create map's record"sv};
  }


  void map_mismatch::throw_exception()
  {
    throw dict_inconsistent_map_mismatch{*this, "dict::add() ~ map already contains record with wrong id"sv};
  }


} // ns
