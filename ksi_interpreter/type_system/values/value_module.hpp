#pragma once

/*
  constant:
    name
    value

  module_data // non-const values (per thread)
*/

/*
  вопрос: Можно ли при вычислении значения "для константы в модуле" читать 'изменяемые свойства' (любого) модуля?
  note: А если там допустим вызов функций, то каких?

  решение:
    на данном этапе значения констант хранятся во thred_space
    Вопрос экономии оперативной памяти для "простых констант" ( vm.config ) оставлю на потом.

  внешние свойства ~ ( properties of module, properties of notion, enum_element.value )

  simple_const
    * Не зависит от изменяемых внешних свойств

  pure_function
    * Не читает изменяемые внешние свойства
    * Не вызывает другие функции, которые приводят к чтению изменямых внешних свойств

  Think about type: $counter(start, step)
*/

#include "bases/value_static.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::values::value_module
    : public system<Type_settings>::bases::value_static
    , public system<Type_settings>::info::meta_info
  {
    using self_meta = info::meta_info;

    // props
    //constants
    info::table_of_properties  property_names;

    // ctor
    value_module(info::meta_info params)
      : self_meta{ params }
    {}

    // actions

    ptr_type get_type(ptr_system_types sys_types) const override;

    t_string_internal get_class_name() const override
    {
      return "value_module"s;
    }
  };


} // ns
