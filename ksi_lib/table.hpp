#pragma once

#include "concepts.hpp"
#include <list>
#include <map>

namespace ksi::lib {


  template <typename T, typename = void>
  struct has_auto_increment : public std::false_type
  {
    static constexpr bool is_appropriate = false;
    using member_type = void;
  };

  template <typename T>
  struct has_auto_increment< T, std::void_t<decltype(T::auto_increment)> >
    : public std::true_type
  {
    using info = ksi::type_traits::pointer_to_member<decltype(T::auto_increment)>;

    static constexpr bool is_appropriate = ksi::concepts::pointer_to_member_of<typename info::source_type, T>;
    using member_type = std::conditional_t<is_appropriate, typename info::member_type, void>;
  };


  template <typename Struct, concepts::pointer_to_member_of<Struct> auto Key_member, typename Less = std::ranges::less>
  struct table
  {
    // note: if no concepts
    //static_assert( std::is_member_pointer_v<Key_member> && std::is_base_of_v<class_of_member_t<Key_member>, Struct> );

    using value_type = Struct;
    using pointer = Struct *;
    using key_type = ksi::type_traits::pointer_to_member<decltype(Key_member)>::member_type;
    using data_type = std::list<Struct>;
    using data_iterator = data_type::iterator;
    using index_type = std::map<key_type, pointer, Less>;
    using index_iterator = index_type::iterator;

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

    pointer find(key_type key, pointer result_not_found = nullptr)
    {
      index_iterator it = index.find(key);
      return ((index.end() == it) ? result_not_found : it->second);
    }

    template <typename Type, typename ... Args>
    pointer emplace_back(std::in_place_type_t<Type>, Args && ... args)
    {
      data_type tmp_data;
      pointer ret;
      if constexpr( auto_increment_need_cast )
      {
        ret = & tmp_data.emplace_back(Type{
          static_cast<auto_increment::member_type>( data.size() ),
          std::forward<Args>(args) ...
        });
      }
      else if constexpr( auto_increment_direct )
      {
        ret = & tmp_data.emplace_back(Type{ data.size(), std::forward<Args>(args) ... });
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
