

struct position
{
  using type = const char *;

  type current, end;

  bool is_end() const { return (current == end); }
};

struct state
{
  static void sample_function(state & st) {}
  using function_type = decltype(&sample_function);

  std::size_t line_number = 0;
  position pos;
  loader::parser_data data;
  sys::string message;

  function_type fn;
};


struct end_of_file
{
  bool check(state & st)
  {
    return st.pos.is_end();
  }
};


struct is_name
{
  sys::string result;

  bool check(state & st)
  {
    if( std::isalpha(*st.pos.current) )
    {
      position & pos = st.pos;
      position::type begin = pos.current;

      do
      {
        ++pos.current;
        if( pos.is_end() ) { break; }
      }
      while( std::isalnum(*pos.current) );

      result.assign(begin, pos.current);
    }

    return (result.size() > 0);
  }
};


template <sys::character Ch>
struct keyword
{
  sys::string result;

  bool check(state & st)
  {
    if( *st.pos.current != Ch ) { return false; }

    position::type begin = st.pos.current;
    position & pos = st.pos;

    do
    {
      ++pos.current;
      if( pos.is_end() ) { break; }
    }
    while( std::isalpha(*pos.current) );

    if( begin + 1 == pos.current ) { return false; }

    result.assign(begin, pos.current);

    return true;
  }
};


template <typename Token, typename Action>
struct is_place
{
  static void function(state & st)
  {
    Token t;
    if( t.check(st) )
    {
      Action::perform(st, t.result);
    } else {
      st.message = "Token not recognized";
    }
  }
};

