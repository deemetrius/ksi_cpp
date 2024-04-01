#pragma once

#include <string>
#include <string_view>

namespace ksi::conv {

  template <typename Result>
  struct string_cast;


  template <>
  struct string_cast<std::wstring>
  {
    using result_type = std::wstring;

    result_type operator () (std::wstring param) const
    {
      return param;
    }

    result_type operator () (std::string_view param) const
    {
      std::mbstate_t state = std::mbstate_t();

      const char * src = param.data();
      std::size_t len{ 0 };
      if( mbsrtowcs_s(&len, nullptr, 0, &src, param.size(), &state) )
      {
        return {};
      }

      std::wstring ret{ len - 1, L'\0', std::allocator<wchar_t>{} };
      if( mbsrtowcs_s(nullptr, ret.data(), len, &src, param.size(), &state) )
      {
        return {};
      }

      return ret;
    }

    result_type operator () (std::string param) const
    {
      std::string_view sv{ param };
      return (*this)(sv);
    }
  };


  template <>
  struct string_cast<std::string>
  {
    using result_type = std::string;

    result_type operator () (std::string param) const
    {
      return param;
    }

    result_type operator () (std::wstring_view param) const
    {
      std::mbstate_t state = std::mbstate_t();

      const wchar_t * src = param.data();
      std::size_t len{ 0 };
      if( wcsrtombs_s(&len, nullptr, 0, &src, param.size(), &state) )
      {
        return {};
      }

      std::string ret{ len - 1, '\0', std::allocator<char>{} };
      if( wcsrtombs_s(nullptr, ret.data(), len, &src, param.size(), &state) )
      {
        return {};
      }

      return ret;
    }

    result_type operator () (std::wstring param) const
    {
      std::wstring_view sv{ param };
      return (*this)(sv);
    }
  };

} // end ns
