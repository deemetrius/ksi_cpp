#pragma once

#include <optional>
#include <functional>

namespace ksi::lib {

struct none {};

template <typename Result>
struct conv_default
{
  template <typename Source>
  Result operator () (Source param) const
  {
    return static_cast<Result>(param);
  }
};

template <typename Prim, typename Opt>
struct dual
{
  using prim_type = Prim;

  using second_type = Opt;
  using optional_type = std::optional<Opt>;
  
  using converter = std::function<Opt (Prim)>;
  using back_converter = std::function<Prim (Opt)>;


  prim_type val;
  optional_type opt;
  converter to_second{ conv_default<Opt>{} };
  back_converter to_first{ conv_default<Prim>{} };


  second_type & second()
  {
    if( opt.has_value() == false )
    {
      opt = to_second(val);
    }
    return opt.value();
  }

  prim_type & first()
  {
    if( opt.has_value() )
    {
      val = to_first( opt.value() );
    }
    return val;
  }
};

} // end ns
