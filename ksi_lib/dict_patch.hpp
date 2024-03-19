#pragma once

#include "dict.hpp"

namespace ksi::lib {


  template <typename String>
  struct dict_patch
  {
    using dict_type = dict<String>;
    using ptr_dict_type = std::shared_ptr<dict_type>;
    using index_type = dict_index_type;

    using set_type = dict_type::set_type;
    using iterator = dict_type::iterator;
    using term_view_type = dict_type::term_view_type;
    using term_type = dict_type::term_type;

    using result_has = dict_type::result_has;
    using result_add = dict_type::result_add;

    enum class error_apply { target_changed, duplicates_found };

    // props
    ptr_dict_type target;
    dict_type     extra;

    // ctor
    dict_patch(ptr_dict_type source_dict)
      : target{ std::move(source_dict) }
      , extra{ .rank_initial = target->set.size() }
    {}

    // actions

    result_has has(term_view_type term) const
    {
      result_has ret{ extra.has(term) };
      return (ret.absent ? target->has(term) : ret);
    }

    result_add add(term_type term)
    {
      result_has res = target->has(term);
      if( res.absent ) { return extra.add( std::move(term) ); }
      return {res.it, false};
    }

    void apply()
    {
      if( target->set.size() != extra.rank_initial ) { throw error_apply::target_changed; }
      target->set.merge(extra.set);
      if( extra.set.size() != 0 ) { throw error_apply::duplicates_found; }
    }
  };


} // ns
