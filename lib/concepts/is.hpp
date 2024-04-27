#pragma once

  #include <type_traits>

namespace ksi::lib::concepts {

  template <typename Type, typename What>
  concept is = std::is_same_v<What, Type>;

} // ns
