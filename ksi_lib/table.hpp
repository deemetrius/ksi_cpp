#pragma once

#include <type_traits>
#include <list>
#include <map>

namespace ksi::lib {


  template <typename T>
  struct class_of_member
  {
    using type = void;
  };

  template <typename T, typename Class>
  struct class_of_member<T Class::*>
  {
    using type = Class;
    using member_type = T;
  };

  template <typename T>
  using class_of_member_t = class_of_member<T>::type;

  template <typename T, typename Struct>
  concept pointer_to_member = std::is_member_pointer_v<T> && std::is_base_of_v<class_of_member_t<T>, Struct>;


  template <typename Struct, pointer_to_member<Struct> auto Key_member, typename Less = std::ranges::less>
  struct table
  {
    // note: if no concepts
    //static_assert( std::is_member_pointer_v<Key_member> && std::is_base_of_v<class_of_member_t<Key_member>, Struct> );

    using key_type = class_of_member<decltype(Key_member)>::member_type;
    using data_type = std::list<Struct>;
    using data_iterator = data_type::iterator;
    using index_type = std::map<key_type, data_iterator, Less>;

    // props
    data_type   data;
    index_type  index;

    template <typename ... Args>
    void emplace_back(Args && ... args)
    {
      data_iterator it = data.emplace(data.end(), std::forward<Args>(args) ...);
      index.insert({(*it).*Key_member, it});
    }
  };


} // ns
