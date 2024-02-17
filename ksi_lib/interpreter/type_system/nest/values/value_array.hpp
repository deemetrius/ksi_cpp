#pragma once

#include "../care/slot.hpp"
#include <vector>

namespace ksi::interpreter {


  using namespace std::string_literals;


  template <typename Type_config>
  struct types<Type_config>::value_array
    : public types<Type_config>::bases::value_pointed
  {
    using storage_type = std::vector<typename care::slot>;
    using size_type = typename storage_type::size_type;

    // props
    storage_type storage;

    // ctor
    value_array(size_type reserve_amount)
    {
      storage.reserve(reserve_amount);
    }

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_array"s;
    }
  };


} // ns
