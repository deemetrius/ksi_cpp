

struct end_of_file
{
  bool check(loader::state & st)
  {
    return st.pos.is_end();
  }
};


template <char Ch>
struct is_char
{
  sys::string result;

  bool check(loader::state & st)
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

  bool check(loader::state & st)
  {
    if( std::isalpha(*st.pos.current) )
    {
      loader::position & pos = st.pos;
      loader::position::type begin = pos.current;

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
  static inline const sys::string name{ "digits"s };

  sys::string result;

  bool check(loader::state & st)
  {
    if( std::isdigit(*st.pos.current) )
    {
      loader::position & pos = st.pos;
      loader::position::type begin = pos.current;
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

  bool check(loader::state & st)
  {
    if( *st.pos.current != Ch ) { return false; }

    loader::position::type begin = st.pos.current;
    loader::position & pos = st.pos;

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

  static loader::state::fn_result function(loader::state & st)
  {
    Token t;
    if( t.check(st) )
    {
      Action::perform(st, t.result);
    } else {
      st.message = ("Token not recognized: "s + Token::name);
    }

    if( st.pos.is_end() )
    {
      if( is_last )
      {
        st.message = "Expected next token"s;
        return loader::state::fn_result::fine_exit;
      }
      return loader::state::fn_result::error_occured;
    }

    return loader::state::fn_result::keep_continue;
  }
};

