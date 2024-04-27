#pragma once

  #include <type_traits>

namespace ksi::lib::detail {


  template <typename Row, typename = void>
  struct table_row
  {
    template <typename Vector>
    static void auto_increment_maybe(Row * row, Vector const & vec) {}
  };


  template <typename Row>
  struct table_row<Row, std::void_t<decltype(Row::auto_increment)> >
  {
    template <typename Vector>
    static void auto_increment_maybe(Row * row, Vector const & vec)
    {
      row->*(Row::auto_increment) = vec.size();
    }
  };


}