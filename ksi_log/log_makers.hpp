#pragma once

  #include <memory>
  #include "logger_none.hpp"
  #include "logger_to_file.hpp"
  #include <utility>

namespace ksi::log {
  using namespace std::string_view_literals;


  struct maker_none
  {
    template <typename Log_Record, typename Writer_Nest>
    using result_type = std::shared_ptr< logger_none<Log_Record> >;

    template <typename Log_Record, typename Writer_Nest>
    result_type<Log_Record, Writer_Nest> operator () (std::in_place_type_t<Log_Record>, std::in_place_type_t<Writer_Nest>) const
    {
      return std::make_shared< logger_none<Log_Record> >();
    }
  };


  template <typename Log_Params>
  struct maker_to_file
  {
    std::string       file_path;

    template <typename Log_Record, typename Writer_Nest>
    using result_type = std::shared_ptr< logger_to_file<Log_Record, Writer_Nest> >;

    template <typename Log_Record, typename Writer_Nest>
    result_type<Log_Record, Writer_Nest> operator () (std::in_place_type_t<Log_Record>, std::in_place_type_t<Writer_Nest>) const
    {
      using writer_type = Writer_Nest::template functor<Log_Record>;

      return std::make_shared< logger_to_file<Log_Record, Writer_Nest> >( file_path, writer_type{Log_Params::format_message, Log_Params::format_source_location} );
    }
  };


} // ns
