#pragma once

#include "../care/slot.hpp"
#include "../care/holder_cell.hpp"
#include "bases/value_pointed.hpp"
#include <vector>
#include <type_traits>

namespace ksi::interpreter {
  using namespace std::string_literals;


  template <typename Type_settings>
  struct system<Type_settings>::values::value_array
    : public system<Type_settings>::bases::value_pointed
  {
    using storage_type = std::vector<typename care::slot>;
    using size_type = typename storage_type::size_type;

    // props
    storage_type properties;

    // ctor
    template <typename Tag = care::tag_none>
    value_array(size_type reserve_amount, Tag = care::is_transitive)
    {
      if constexpr( std::is_same_v<Tag, typename care::tag_root> )
      {
        this->point.is_root = true;
      }
      properties.reserve(reserve_amount);
    }

    ~value_array()
    {
      for( typename care::slot & it : properties )
      {
        typename care::holder_cell keep_closer{ it.release() };
      }
    }

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string_internal get_class_name() const override { return "value_array"s; }

    void append(care::holder_cell && keep_cell)
    {
      typename storage_type::iterator it{ properties.emplace_back(keep_cell.release(), & this->point) };
      it->cell_handle->junction_point.refs_entrain(& this->point);
    }
  };


} // ns
