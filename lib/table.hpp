#pragma once

  #include <list>
  #include <map>
  #include <vector>
  #include "concepts/member_pointer_of.hpp"
  #include "traits/determine_member_type.hpp"

  #include "table_row.hpp"

namespace ksi::lib {

  template <
    typename Struct,
    concepts::member_pointer_of<Struct> auto Index,
    typename Compare = std::ranges::less
  >
  struct table
  {
    using key_type = traits::determine_member_type<decltype(Index)>::result;
    using pointer = Struct *;
    using map_type = std::map<key_type, pointer, Compare>;

    using subscript = std::vector<pointer>;

    std::list<Struct>   rows;
    map_type            index;
    subscript           pos;

    template <typename Param, typename ... Args>
    pointer append_row(Args ... args)
    {
      std::list<Struct> row;
      pointer ret = &row.emplace_back( Param{std::move(args) ...} );

      map_type row_index;
      row_index.emplace_hint(row_index.end(), ret->*Index, ret);

      detail::table_row<Struct>::auto_increment_maybe(ret, pos);
      pos.push_back(ret);

      rows.splice(rows.end(), row);
      index.merge(row_index);

      return ret;
    }
  };

} // ns
