#pragma once

  #include <utility>
  #include <cstdio>
  #include <cstdint>

namespace ksi::files {


  struct open_modes
  {
    using flag_type = std::uint16_t;
    struct flags
    {
      flag_type   value;

      enum open_flags : flag_type {
        action_read   = 0b01,
        action_write  = 0b10,
        action_both   = 0b11,

        if_exists_from_start  = 0b00'00,
        if_exists_to_end      = 0b01'00,
        if_exists_make_empty  = 0b10'00,
        if_exists_not_open    = 0b11'00,

        when_missing_no_create    = 0b0'00'00,
        when_missing_can_make_new = 0b1'00'00,
      };

      bool can_read() const { return (value & action_read); }
      bool can_write() const { return (value & action_write); }
    };

    enum class mode { text = 0, binary = 1 };

    struct info_type
    {
      using raw_mode_type = const char *;

      raw_mode_type   internal;
      mode            type;
      flags           info;
    };

    struct pack { info_type  binary, text; };

    static constexpr pack
      open_read_existing{
        .binary { "rb", mode::binary, flags::action_read | flags::if_exists_from_start | flags::when_missing_no_create },
        .text   { "r" , mode::text  , flags::action_read | flags::if_exists_from_start | flags::when_missing_no_create }
      },
      open_write_new_or_overwite{
        .binary { "wb", mode::binary, flags::action_write | flags::if_exists_make_empty | flags::when_missing_can_make_new },
        .text   { "w" , mode::text  , flags::action_write | flags::if_exists_make_empty | flags::when_missing_can_make_new }
      },
      open_write_new_or_append{
        .binary { "ab", mode::binary, flags::action_write | flags::if_exists_to_end | flags::when_missing_can_make_new },
        .text   { "a" , mode::text  , flags::action_write | flags::if_exists_to_end | flags::when_missing_can_make_new }
      },

      open_rw_existing_from_start_no_create{
        .binary { "r+b", mode::binary, flags::action_both | flags::if_exists_from_start | flags::when_missing_no_create },
        .text   { "r+" , mode::text  , flags::action_both | flags::if_exists_from_start | flags::when_missing_no_create }
      },
      open_rw_new_or_overwite{
        .binary { "w+b", mode::binary, flags::action_both | flags::if_exists_make_empty | flags::when_missing_can_make_new },
        .text   { "w+" , mode::text  , flags::action_both | flags::if_exists_make_empty | flags::when_missing_can_make_new }
      },
      open_rw_new_or_append{
        .binary { "a+b", mode::binary, flags::action_both | flags::if_exists_to_end | flags::when_missing_can_make_new },
        .text   { "a+" , mode::text  , flags::action_both | flags::if_exists_to_end | flags::when_missing_can_make_new }
      },

      open_write_new_only{
        .binary { "wxb", mode::binary, flags::action_write | flags::if_exists_not_open | flags::when_missing_can_make_new },
        .text   { "wx" , mode::text  , flags::action_write | flags::if_exists_not_open | flags::when_missing_can_make_new }
      },
      open_rw_no_overwrite{
        .binary { "w+xb", mode::binary, flags::action_both | flags::if_exists_not_open | flags::when_missing_can_make_new },
        .text   { "w+x" , mode::text  , flags::action_both | flags::if_exists_not_open | flags::when_missing_can_make_new }
      }
    ;
  };


  enum class std_marker { std_input, std_output, std_error };


  struct file_handle
  {
    using handle_type = std::FILE *;
    using path_type = const char *;

  private:
    struct traits
    {
      static inline handle_type std[3]{ stdin, stdout, stderr };

      static bool close_normal(handle_type handle) { return (std::fclose(handle) == 0); }
      static bool no_close(handle_type handle) { return false; }
    };

  public:
    using closer_type = decltype(& traits::close_normal);

    // props
    handle_type   handle;
    closer_type   closer;

    ~file_handle()
    {
      if( closer(handle) ) { handle = nullptr; }
    }

    file_handle(std_marker num)
      : handle{ traits::std[static_cast<std::size_t>(num)] }
      , closer{ & traits::no_close }
    {}

    file_handle(handle_type file_handle)
      : handle{ file_handle }
      , closer{ & traits::close_normal }
    {}

    file_handle(file_handle const &) = delete;
    file_handle & operator = (file_handle const &) = delete;
  };


} // ns
