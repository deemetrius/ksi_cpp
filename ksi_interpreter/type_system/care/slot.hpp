#pragma once

#include "holder_cell.hpp"

namespace ksi::interpreter {


  template <typename Type_settings>
  struct system<Type_settings>::care::slot
  {
    // props
    ptr_cell                  cell_handle;
    ptr_point                 point_handle;
    info::ptr_property_info   meta_information{ nullptr };

    void assign(care::holder_cell && from_keep)
    {
      if( keep_cell.empty() ) { throw errors::assign_from_empty{"slot"}; }

      from_keep.cell_handle->junction_point.refs_entrain(point_handle);
      care::holder_cell to_keep{
        std::exchange(cell_handle, from_keep.release())
      };
      to_keep.cell_handle->junction_point.refs_detrain(point_handle);
    }

    ptr_cell release()
    {
      return std::exchange(cell_handle, nullptr);
    }
  };


} // ns
