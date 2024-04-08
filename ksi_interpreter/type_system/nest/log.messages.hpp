#pragma once

  #include "log.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::log::keys
  {
    static constexpr message_key
      cat_circular_dependencies         { log_level::error,  301 },
      cat_set_as_parent_again_for_cats  { log_level::notice, 302 },
      cat_already_assigned_for_types    { log_level::notice, 303 }
    ;
  };


  template <typename Type_settings>
  struct system<Type_settings>::log::messages
  {
    static inline const log::message
      intro{
        converter_string("Started привет"sv),
        { log_level::info, 1 }
      }
    ;
  };


} // ns
