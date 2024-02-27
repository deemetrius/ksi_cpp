#pragma once

#include "../care/slot.hpp"
#include "../care/holder_cell.hpp"
#include <vector>
#include <type_traits>

namespace ksi::interpreter {
  using namespace std::string_literals;


  template <typename Type_config>
  struct types<Type_config>::values::value_array
    : public types<Type_config>::bases::value_pointed
  {
    using storage_type = std::vector<typename care::slot>;
    using size_type = typename storage_type::size_type;

    // props
    storage_type storage;

    // ctor
    template <typename Tag = care::tag_none>
    value_array(size_type reserve_amount, Tag = care::is_transitive)
    {
      if constexpr( std::is_same_v<Tag, typename care::tag_root> )
      {
        this->point.is_root = true;
      }
      storage.reserve(reserve_amount);
    }

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;
    t_string_internal get_class_name() const override { return "value_array"s; }

    void append(care::ptr_cell cell_handle, care::holder_cell && keep_cell)
    {
      typename storage_type::iterator it{ storage.emplace_back(keep_cell.release(), & this->point) };
      it->cell_handle->junction_point.refs_entrain(& this->point);
    }
  };


} // ns
