#pragma once

#include <set>
#include <string_view>

  // big todo: dict ~ struct result_add{ bool already_exists; }

namespace ksi::lib {


  using dict_index_type = std::size_t;


  template <typename String>
  struct dict
  {
    using term_type = String;
    using term_view_type = std::basic_string_view<typename term_type::value_type>;
    using index_type = dict_index_type;

    //static constexpr index_type rank_initial{ 0 };

    struct value_type
    {
      using const_pointer = value_type const *;

      // props
      term_type           term;
      index_type          id;
      mutable index_type  rank;

      // actions

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

    using const_pointer = value_type const *;
    using set_type = std::set<value_type, less>;
    using iterator = typename set_type::iterator;

    struct result_has
    {
      iterator it;
      bool absent;

      bool included() const { return (not absent); }
    };

    struct result_add
    {
      iterator it;
      bool was_added;
    };

    // props
    set_type    set;
    index_type  rank_initial{ 0 };

    // actions

    result_has has(term_view_type term) const
    {
      iterator it{ set.find(term) };
      return {it, it == set.end()};
    }

    result_add add(term_type term)
    {
      auto [lower, upper] = set.equal_range(term_view_type{term});
      if( lower == upper )
      {
        iterator it = set.emplace_hint(upper, std::move(term), traits::next_id(this), traits::rank_of(this, upper));
        traits::rank_renumerate_after(this, it);
        return {it, true};
      }
      return {lower, false};
    }

    struct traits
    {
      using dict_pointer = dict *;
      using const_dict_pointer = dict const *;

      static index_type next_id(const_dict_pointer self)
      {
        return (self->set.size() + self->rank_initial);
      }

      static index_type rank_of(dict_pointer self, iterator it)
      {
        return (
          (self->set.end() == it) ? (self->set.size() + self->rank_initial) : it->rank
        );
      }

      /// @note self->set.end() is not allowed as param ~ it
      static void rank_renumerate_after(dict_pointer self, iterator it)
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
