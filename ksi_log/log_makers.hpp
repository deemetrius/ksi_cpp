#pragma once

  #include <memory>
  #include "logger_none.hpp"
  #include "logger_to_file.hpp"
  #include <utility>

namespace ksi::log {
  using namespace std::string_view_literals;


  struct maker_none
  {
    template <typename Record, typename Writer>
    using result_type = std::shared_ptr< logger_none<Record> >;

    template <typename Record, typename Writer>
    result_type<Record, Writer> operator () (std::in_place_type_t<Record>, std::in_place_type_t<Writer>) const
    {
      return std::make_shared< logger_none<Record> >();
    }
  };


  template <typename Params>
  struct maker_to_file
  {
    std::string  file_path;

    template <typename Record, typename Writer>
    using result_type = std::shared_ptr< logger_to_file<Record, Writer> >;

    template <typename Record, typename Writer>
    result_type<Record, Writer> operator () (std::in_place_type_t<Record>, std::in_place_type_t<Writer>) const
    {
      using writer_type = Writer::template functor<Record>;

      return std::make_shared< logger_to_file<Record, Writer> >(
        file_path,
        writer_type{Params::format_message, Params::format_source_location}
      );
    }
  };


} // ns
