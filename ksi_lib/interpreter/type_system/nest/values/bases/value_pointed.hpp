#pragma once

#include "value_managed.hpp"
#include "../../care/root_finder.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::bases::value_pointed
    : public types<Type_config>::bases::value_managed
  {
    // props
    care::point point;

    // actions

    void was_acquired(care::ptr_cell by_cell) override
    {
      if( point.rel_add(by_cell) == false )
      {
        // todo: log
      }
    }

    void was_redeemed(care::ptr_cell by_cell) override
    {
      if( point.rel_del(by_cell) == false )
      {
        // todo: log
      }
    }

    care::value_status determine_status() const override
    {
      if( point.rels_empty() ) { return care::value_status::n_ready_for_delete; }
      try
      {
        care::root_finder finder;
        return (
          finder.find_for_point(&point) ?
          care::value_status::n_should_stay :
          care::value_status::n_holded_by_only_circular_refs
        );
      }
      catch( std::bad_alloc const & e )
      {
        // todo: chain 'this' value as doubtful
        return care::value_status::n_undetermined;
      }
    }
  };


} // ns
