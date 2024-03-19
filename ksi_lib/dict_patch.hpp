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

    using result_has = dict_type::result_has;
    using result_add = dict_type::result_add;

    // props
    ptr_dict_type target_dict;
    index_type    rank_initial;
    dict_type     extra;

    // ctor
    dict_patch(ptr_dict_type source_dict)
      : target_dict{ std::move(source_dict) }
      , rank_initial{ source_dict->set.size() }
    {}

    // actions

    result_has has(term_view_type term) const
    {
      result_type ret{ extra.has(term) };
      return (ret.absent ? target_dict->has(term) : ret);
    }
  };


} // ns
