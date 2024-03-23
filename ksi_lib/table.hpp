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

  template <typename T, typename = void>
  struct has_auto_increment : public std::false_type
  {
    using member_type = void;
  };

  template <typename T>
  struct has_auto_increment< T, std::void_t<decltype(T::auto_increment)> > : public std::true_type
  {
    using member_type = decltype(std::declval<T>() .* T::auto_increment);
  };

  template <typename Struct, pointer_to_member<Struct> auto Key_member, typename Less = std::ranges::less>
  struct table
  {
    // note: if no concepts
    //static_assert( std::is_member_pointer_v<Key_member> && std::is_base_of_v<class_of_member_t<Key_member>, Struct> );

    using value_type = Struct;
    using pointer = Struct *;
    using key_type = class_of_member<decltype(Key_member)>::member_type;
    using data_type = std::list<Struct>;
    using data_iterator = data_type::iterator;
    using index_type = std::map<key_type, pointer, Less>;

    using auto_increment = has_auto_increment<Struct>;

    static constexpr bool
    auto_increment_direct{ std::is_same_v<
      typename data_type::size_type,
      typename auto_increment::member_type
    > };

    static constexpr bool
    auto_increment_need_cast{
      auto_increment::value && (not auto_increment_direct)
    };

    // props
    data_type   data;
    index_type  index;

    template <typename Type, typename ... Args>
    pointer emplace_back(std::in_place_type_t<Type>, Args && ... args)
    {
      data_type tmp_data;
      pointer ret;
      if constexpr( auto_increment_direct )
      {
        ret = & tmp_data.emplace_back(Type{ data.size(), std::forward<Args>(args) ... });
      }
      else if constexpr( auto_increment_need_cast )
      {
        ret = & tmp_data.emplace_back(Type{
          static_cast<auto_increment::member_type>( data.size() ),
          std::forward<Args>(args) ...
        });
      }
      else
      {
        ret = & tmp_data.emplace_back(Type{ std::forward<Args>(args) ... });
      }
      index.insert({ret->*Key_member, ret});
      data.splice(data.end(), tmp_data);
      return ret;
    }
  };


} // ns
