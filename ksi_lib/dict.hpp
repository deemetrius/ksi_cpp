#pragma once

#include <set>
#include <string_view>

namespace ksi::lib {


  using dict_index_type = std::size_t;


  template <typename String>
  struct dict
  {
    using key_type = String;
    using term_type = std::basic_string_view<typename key_type::value_type>;
    using index_type = dict_index_type;

    struct value_type
    {
      key_type            key;
      mutable index_type  index;
    };

    struct less
    {
      bool operator () (value_type const & v1, value_type const & v2) const
      {
        return (v1.key < v2.key);
      }

      bool operator () (value_type const & v1, term_type v2) const
      {
        return (v1.key < v2);
      }

      bool operator () (term_type v1, value_type const & v2) const
      {
        return (v1 < v2.key);
      }

      enum is_transparent {};
    };

    using set_type = std::set<value_type, less>;
    using iterator = typename set_type::iterator;

    struct result_type
    {
      iterator it;
      bool was_added;
    };

    // props
    set_type set;

    result_type has(term_type key) const
    {
      iterator it{ set.find(key) };
      return {it, it != set.end()};
    }

    result_type add(key_type key)
    {
      auto [lower, upper] = set.template equal_range<term_type>(key);
      if( lower == upper )
      {
        iterator it = set.emplace_hint(upper, std::move(key), traits::calc_index(this, upper));
        traits::reindex(this, it);
        return {it, true};
      }
      return {lower, false};
    }

    struct traits
    {
      using dict_pointer = dict *;

      static index_type calc_index(dict_pointer self, iterator it)
      {
        return (
          (self->set.begin() == it) ? 0 : (
            (self->set.end() == it) ? (self->set.crbegin()->index + 1) : it->index
          )
        );
      }

      static void reindex(dict_pointer self, iterator it)
      {
        index_type index = it->index;
        while( (++it) != self->set.end() )
        {
          it->index = (++index);
        }
      }
    };
  };


} // ns
