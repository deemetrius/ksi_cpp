#pragma once

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_settings>
  struct system<Type_settings>::values::value_cat
    : public system::bases::value_static
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // maybe: decide is better to throw
    enum class result_add_base { circular_cats_forbidden, same_twice, success };

    // props
    info::cat_includes base_categories;

    // ctor
    value_cat(info::meta_info params)
      : self_meta{ std::move(params) }
    {}

    // sub-cat actions

    bool cat_base_has(ptr_cat base_cat_handle) const
    {
      return base_categories.all_includes.contains(base_cat_handle);
    }

    result_add_base cat_base_add(ptr_cat base_cat_handle)
    {
      if( base_cat_handle->cat_base_has(this) ) { return result_add_base::circular_cats_forbidden; }
      typename info::cat_includes::result_add res = base_categories.add(base_cat_handle);
      return (
        (res == info::cat_includes::result_add::was_already_included) ?
        result_add_base::same_twice :
        result_add_base::success
      );
    }

    // value actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_cat"s;
    }
  };


  template <typename Type_settings>
  auto system<Type_settings>::info::cat_includes::add(ptr_cat base_cat_handle) -> result_add
  {
    auto [it_lower, it_upper] = this->includes_directly.equal_range(base_cat_handle);
    if( it_lower != it_upper ) { return result_add::was_already_included; }

    info::cat_set all_tmp = base_cat_handle->base_categories.all_includes;
    all_tmp.insert(base_cat_handle);

    this->includes_directly.insert(it_upper, base_cat_handle);
    this->all_includes.merge(all_tmp);

    return result_add::just_added;
  }


} // ns
