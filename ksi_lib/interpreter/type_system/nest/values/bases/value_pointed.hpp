#pragma once

#include "value_managed.hpp"
#include "../../care/point.hpp"

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
      // big todo:
    }
  };


} // ns
