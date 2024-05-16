

struct position
{
  using type = const char *;

  type current, end;

  bool is_end() const { return (current == end); }
};

struct prepare_type
{
  sys::string name;
  sys::unique<execution::sequence> seq = std::make_unique<execution::sequence>();

  execution::group_instructions & seq_current_group()
  {
    if( seq->groups.empty() )
    {
      seq->groups.emplace_back();
    }

    return seq->groups.front();
  }
};

struct state
{
  static void sample_function(state & st) {}
  using function_type = decltype(&sample_function);

  // props

  std::size_t line_number = 0;
  position    pos;

  loader::parser_data   data;
  prepare_type          prepare;

  function_type fn;

  sys::string message;
  bool        is_done = false;
};


struct end_of_file
{
  bool check(state & st)
  {
    return st.pos.is_end();
  }
};


template <char Ch>
struct is_char
{
  bool check(state & st)
  {
    if( *st.pos.current == Ch )
    {
      ++st.pos.current;
      return true;
    }

    return {};
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

struct digits
{
  sys::string result;

  bool check(state & st)
  {
    if( std::isdigit(*st.pos.current) )
    {
      position & pos = st.pos;
      position::type begin = pos.current;
      ++pos.current;

      while( (pos.is_end() == false) && std::isdigit(*pos.current) )
      {
        ++pos.current;
      }

      result.assign(begin, pos.current);
      return true;
    }

    return false;
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


enum class is_last_rule {};

template <typename Token, typename Action, typename IsLast = void>
struct is_place
{
  static constexpr bool is_last = std::is_same_v<IsLast, is_last_rule>;

  static void function(state & st)
  {
    Token t;
    if( t.check(st) )
    {
      Action::perform(st, t.result);
    } else {
      st.message = "Token not recognized";
    }

    st.is_done = is_last;
  }
};

