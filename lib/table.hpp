#pragma once

  #include <list>
  #include <map>
  #include <vector>
  #include "concept.member_pointer_of.hpp"
  #include "helper.determine_member_type.hpp"

  #include "table_row.hpp"

namespace ksi::lib {

  struct table_key_not_unique {};

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

    pointer find(key_type key)
    {
      typename map_type::iterator it = index.find(key);
      return ((index.end() == it) ? nullptr : it->second);
    }

    struct append_result
    {
      pointer result;
      bool    is_same;
    };

    template <typename Param, typename ... Args>
    append_result append_row(Args ... args)
    {
      std::list<Struct> row_temprary;
      pointer h_result = &row_temprary.emplace_back( Param{std::move(args) ...} );

      typename map_type::iterator it = index.find(h_result->*Index);
      if( it != index.end() ) { return {it->second, true}; }

      map_type row_index;
      row_index.emplace(h_result->*Index, h_result);

      detail::table_row<Struct>::auto_increment_maybe(h_result, this->pos);

      rows.splice(rows.end(), row_temprary);
      index.merge(row_index);

      if( row_index.empty() && row_temprary.empty() )
      this->pos.push_back(h_result);

      return {h_result, false};
    }
  };

} // ns
