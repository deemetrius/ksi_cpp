#pragma

  #include "configuration.hpp"
  #include "ksi_lib/table_addon.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::patch
  {
    using addon_of_types = ksi::lib::table_addon<
     typename configuration::table_of_types,
      typename values::value_type,
       & values::value_type::name,
      typename info::literal_less_id
    >;
  };


} // ns

/*
  explanation:

  values::system_types is part of 'space_configuration' by value
  VM .config property is unique_ptr

  table_addon contains raw pointer to source table (type & cats) registration

  We install addons to source tables directly (pointers to elements remains valid)
  read access to space_configuration seems should be preserved by apply patch operation
*/
