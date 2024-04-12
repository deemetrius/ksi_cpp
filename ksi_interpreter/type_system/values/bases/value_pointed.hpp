#pragma once

#include "value_managed.hpp"
#include "../../care/root_finder.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::bases::value_pointed
    : public system<Type_settings>::bases::value_managed
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

    care::status_of_value determine_status() const override
    {
      return (
        point.rels_empty() ?
        care::status_of_value::n_ready_for_delete :
        care::status_of_value::n_requires_point_examination_refs_circular_only
      );
    }

    bases::ptr_value_pointed try_get_pointed() override
    {
      return this;
    }
  };


} // ns
