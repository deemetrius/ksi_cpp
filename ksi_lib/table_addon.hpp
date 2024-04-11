#pragma once

  #include "table.hpp"

namespace ksi::lib {


  template <
    typename Source_table,
    typename Struct,
    concepts::pointer_to_member_of<Struct> auto Key_member,
    typename Less = std::ranges::less
  >
  struct table_addon
  {
    using table_type = table<Struct, Key_member, Less>;
    using ptr_table_type = Source_table *;

    ptr_table_type  source;
    NO_UNIQUE_ADDRESS typename table_type::auto_increment::result  increment_from{ source->data.size() };
    table_type      change;

    template <typename Type, typename ... Args>
    typename table_type::pointer emplace_back(Args && ... args)
    {
      typename table_type::data_type tmp;
      typename table_type::pointer ptr = source->find(
        tmp.emplace_back(std::forward<Args ...>(args ...)).*Key_member
      );

      increment_from.change(ptr, change.data.size());

      return ((nullptr != ptr) ? change.merge_from_list(tmp)->second : ptr);
    }
  };


} // ns
