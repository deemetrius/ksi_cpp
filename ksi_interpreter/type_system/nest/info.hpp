#pragma once

#include "../system_nest.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct system<Type_config>::info
  {
    using dict_type = ksi::lib::dict<t_string>;
      using literal_type = dict_type::value_type const *;
      using dict_ptr_type = std::shared_ptr<dict_type>;
      using token_type = std::size_t;

      struct literal_less
      {
        bool operator () (literal_type lt, literal_type rt) const
        {
          return (lt->rank < rt->rank);
        }
      };

      struct meta_info
      {
        index_type    position; // auto_increment ~ should be the first member for table
        literal_type  name;

        static constexpr index_type meta_info::* auto_increment{ & meta_info::position };
      };

      struct property_info
        : public meta_info
      {
        //type_restriction
      };

      using table_of_properties = ksi::lib::table<property_info, & property_info::name, literal_less>;

      struct module_configuration
        : public meta_info
      {
        //constants
        table_of_properties  property_names;
      };

      using table_of_modules = ksi::lib::table<module_configuration, & module_configuration::name, literal_less>;

      struct var_names;
      struct sequence;

      using ptr_property_info = property_info *; // const?
  };


} // ns
