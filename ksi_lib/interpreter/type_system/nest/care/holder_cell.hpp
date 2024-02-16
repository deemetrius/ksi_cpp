#pragma once

#include "root_finder.hpp"

namespace ksi::interpreter {


  template <typename Type_config>
  struct types<Type_config>::care::holder_cell
  {
    using pointer = care::ptr_cell;

    static void depart_cell(pointer & handle)
    {
      typename care::cell::ptr_farewell_function fn{ handle->fn_way_out };
      fn( std::exchange(handle, nullptr) );
    }

    // props
    pointer cell_handle;

    // ctor: from cell pointer
    holder_cell(pointer pc) : cell_handle{ pc }
    {}

    // ctor: move
    holder_cell(holder_cell && other) : cell_handle{ other.release() }
    {}

    ~holder_cell()
    {
      if( cell_handle == nullptr ) { return; }
      switch( cell_handle->junction_point.refs_empty() )
      {
        case false:
        try
        {
          care::root_finder finder;
          if( finder.is_cell_rooted(cell_handle) ) { break; }
          // todo: collect | sub-close
          depart_cell(cell_handle);
        }
        catch( std::bad_alloc const & e )
        {
          // todo: chain 'cell_handle' as doubtful
        }
        break;

        case true:
        depart_cell(cell_handle);
      }
    }

    // actions

    pointer release()
    {
      if( cell_handle == nullptr ) { throw errors::release_on_empty{"holder_cell"} }
      return std::exchange(cell_handle, nullptr);
    }

    pointer operator -> () const
    {
      return value_handle;
    }

    // ctors: no copy, no default
    holder_cell(holder_cell const &) = delete;
    holder_cell() = delete;

    // assigns: no copy, no move
    holder_cell & operator = (holder_cell const &) = delete;
    holder_cell & operator = (holder_cell &&) = delete;
  };


} // ns
