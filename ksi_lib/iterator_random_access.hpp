#pragma once

#include <iterator>
#include <compare>

namespace ksi::lib {


  enum class direction { n_forward, n_reverse };


  template <typename Carry, direction Direction>
  struct iterator_random_access
  {
    using iterator_category = std::random_access_iterator_tag;

    using value_type = Carry::value_type;
    using difference_type = Carry::difference_type;
    using pointer = Carry::pointer;
    using reference = Carry::reference;

    using ordering = Carry::ordering;

    Carry data;

    constexpr reference operator * ()
    {
      return data.access();
    }

    constexpr pointer operator -> ()
    {
      return data.get();
    }


    constexpr iterator_random_access & operator ++ ()
    {
      if constexpr( Direction == direction::n_forward )
      { data = Carry::advance(data, +1); }
      else
      { data = Carry::recede(data, +1); }
      return *this;
    }

    constexpr iterator_random_access & operator -- ()
    {
      if constexpr( Direction == direction::n_forward )
      { data = Carry::recede(data, +1); }
      else
      { data = Carry::advance(data, +1); }
      return *this;
    }

    constexpr iterator_random_access & operator += (difference_type n)
    {
      if constexpr( Direction == direction::n_forward )
      { data = Carry::advance(data, n); }
      else
      { data = Carry::recede(data, n); }
      return *this;
    }

    constexpr iterator_random_access & operator -= (difference_type n)
    {
      if constexpr( Direction == direction::n_forward )
      { data = Carry::recede(data, n); }
      else
      { data = Carry::advance(data, n); }
      return *this;
    }

    friend constexpr iterator_random_access operator + (const iterator_random_access & it, difference_type n)
    {
      if constexpr( Direction == direction::n_forward )
      { return Carry::advance(it.data, n); }
      else
      { return Carry::recede(it.data, n); }
    }

    friend constexpr iterator_random_access operator - (const iterator_random_access & it, difference_type n)
    {
      if constexpr( Direction == direction::n_forward )
      { return Carry::recede(it.data, n); }
      else
      { return Carry::advance(it.data, n); }
    }

    friend constexpr iterator_random_access operator - (const iterator_random_access & it1, const iterator_random_access & it2)
    {
      if constexpr( Direction == direction::n_forward )
      { return Carry::difference(it1.data, it2.data); }
      else
      { return Carry::difference(it2.data, it1.data); }
    }


    friend constexpr ordering operator <=> (const iterator_random_access & it1, const iterator_random_access & it2)
    {
      if constexpr( Direction == direction::n_forward )
      { return Carry::compare(it1.data, it2.data); }
      else
      { return Carry::compare(it2.data, it1.data); }
    }

    friend constexpr bool operator == (const iterator_random_access & it1, const iterator_random_access & it2)
    {
      if constexpr( Direction == direction::n_forward )
      { return (Carry::compare(it1.data, it2.data) == ordering::equivalent); }
      else
      { return (Carry::compare(it2.data, it1.data) == ordering::equivalent); }
    }
  };


  template <typename From, typename To = From>
  struct is_range
  {
    From from;
    To to;

    From begin() const
    {
      return from;
    }

    To end() const
    {
      return to;
    }
  };

} // end ns
