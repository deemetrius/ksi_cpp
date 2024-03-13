#pragma once

#include <set>
#include <string_view>

namespace ksi::lib {


  using dict_index_type = std::size_t;


  template <typename String>
  struct dict
  {
    using term_type = String;
    using term_view_type = std::basic_string_view<typename term_type::value_type>;
    using index_type = dict_index_type;

    struct value_type
    {
      using pointer = value_type *;
      using const_pointer = const value_type *;

      // props
      term_type           term;
      mutable index_type  rank;

      const_pointer get_const() const { return this; }
    };

    struct less
    {
      bool operator () (value_type const & v1, value_type const & v2) const
      {
        return (v1.term < v2.term);
      }

      bool operator () (value_type const & v1, term_view_type v2) const
      {
        return (v1.term < v2);
      }

      bool operator () (term_view_type v1, value_type const & v2) const
      {
        return (v1 < v2.term);
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

    result_type has(term_view_type term) const
    {
      iterator it{ set.find(term) };
      return {it, it != set.end()};
    }

    result_type add(term_type term)
    {
      auto [lower, upper] = set.template equal_range<term_view_type>(term);
      if( lower == upper )
      {
        iterator it = set.emplace_hint(upper, std::move(term), traits::calc_index(this, upper));
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
            (self->set.end() == it) ? (self->set.crbegin()->rank + 1) : it->rank
          )
        );
      }

      static void reindex(dict_pointer self, iterator it)
      {
        index_type rank = it->rank;
        while( (++it) != self->set.end() )
        {
          it->rank = (++rank);
        }
      }
    };
  };


} // ns
