#pragma once

#include "chars.hpp"
#include <cstddef>

namespace ksi::log {


  using index_type = std::ptrdiff_t;


  struct file_position
  {
    index_type  char_index;
    index_type  line;
    index_type  column;
  };


  struct pos_carry
  {
    struct inner
    {
      bool prev_not_cr{ true };

      void cr_was() { prev_not_cr = false; }
      void cr_clear() { prev_not_cr = true; }
    };

    // props
    index_type      tab_size{ 8 };
    file_position   pos{ 0, 1, 1 };
    inner           params;

    template <typename Char>
    index_type reckon(Char symbol) // returns: amount of virtual chars added
    {
      static_assert( std::is_enum_v< chars::symbols<Char> >, "unknown Char type was given" );
      using info_enum = chars::symbols<Char>;

      index_type ret = 0;
      ++pos.char_index;
      if( symbol == info_enum::cr ) [[unlikely]]
      {
        ret = 1;
        params.cr_was();
        ++pos.line;
        pos.column = 1;
      }
      else
      {
        switch( symbol )
        {
          case info_enum::tab :
            ret = (tab_size - (pos.column - 1) % tab_size);
            pos.column += ret;
          break;

          case info_enum::lf :
            if( params.prev_not_cr )
            {
              ret = 1;
              ++pos.line;
              pos.column = 1;
            }
          break;

          default:
          ret = 1;
          ++pos.column;
        }
        params.cr_clear();
      }
      return ret;
    }
  };


} // ns
