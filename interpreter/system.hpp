#pragma once

  #include <forward_list>

  #include <string>
  #include <string_view>
  #include "lib/counter.hpp"
  #include <cstdint>
  #include "lib/traits/no_copyable.h"
  #include "lib/keep.hpp"
  #include <set>
  #include <map>
  #include <vector>

  #include <utility>
  #include <memory>

  namespace ksi_traits = ksi::lib::traits;

  #include "lib/traits/no_copy_assignable.h"

  #include "lib/dict.hpp"
  #include "log_messages.hpp"
  #include "lib/table.hpp"

namespace ksi::interpreter::system {

  using integer = std::intptr_t;

  using t_counter = ksi::lib::counter<integer>;
  using t_string = std::string;

  using dictionary = lib::dict<t_string>;
  using literal = dictionary::map_type::const_pointer;


  template <typename Kind>
  using static_table = lib::table<Kind, Kind::index, typename Kind::Less>;

  using namespace std::string_view_literals;

  struct cell_info;


  namespace traits {

    template <typename Cell>
    bool cell_is_rooted_condition(Cell * handle)
    {
      using ct = Cell *;
      using pt = typename decltype(handle->junction)::key_type;

      std::set<pt> visited_points;
      std::set<ct> visited_cells{ handle };

      using iterator = typename decltype(handle->junction)::iterator;
      using pair = std::pair<iterator, iterator>;

      std::forward_list<pair> back_stack;
      back_stack.emplace_front( handle->junction.begin(), handle->junction.end() );

      do
      {
        for( pair * h = &back_stack.front(); h->first != h->second; ++h->first )
        {
          pt point{ h->first->first };
          if( point->empty() ) { return false; }
          if( visited_points.contains(point) ) { continue; }

          for( ct h_cell : *point )
          if( ! visited_cells.contains(h_cell) )
          {
            back_stack.emplace_front( h_cell->junction.begin(), h_cell->junction.end() );
            visited_cells.insert(h_cell);
          }

          visited_points.insert(point);
        }

        back_stack.pop_front();
      }
      while( ! back_stack.empty() );

      return true;
    }


    template <typename Slot, typename Hint, typename Point>
    struct array_impl
    {
        Point point;
        Hint  type_hint;
      std::vector<Slot> elements;

      array_impl(integer n)
      {
        elements.reserve( static_cast<std::size_t>(n) );
      }

      template <typename Holder>
      void add(Holder from)
      {
        this->elements.emplace_back( &this->point ).accept( from.release() );
      }

      template <typename Holder>
      void sweep(std::list<Holder> & to_box)
      {
        for( Slot & it : elements )
        {
          if( it.uncell() || cell_is_rooted_condition(it.handle) )
          {
            to_box.emplace_front(it.handle);
          }
        }
      }
    };

    struct inner
    {
      template <typename Value, typename To>
      static void assign_placed(Value * from, To * cell);

      template <typename Value, typename To>
      static void assign_static(Value * from, To * cell);
    };

    template <typename Pointer>
    void close_cell_value(Pointer of_cell)
    {
      if( auto fn = of_cell->keep.handle->redeem(of_cell); fn != nullptr )
      {
        fn(of_cell);
      }
    }

    void simple_closer(cell_info *);

    template <typename T>
    void delete_function( T * );


  } // ns


  namespace system_types::values {
    struct type;
    struct category;
    struct module_config;

    struct i_restriction
    {
      virtual bool match(const type * tp) const = 0;
    };
  }

  using restriction_type = system_types::values::i_restriction const *;

  using type_pointer = system_types::values::type const *;

  struct type_info
  {
    literal  name;
    integer  type_index;

    std::size_t position;
  };


  namespace system_types
  {
    struct system_data;

    struct i_value
    {
      using closer = void(*)(cell_info *);

      virtual ~i_value() {}
      virtual type_pointer get_type(system_data * panel) const = 0;

      virtual void assign(cell_info * to_cell) = 0;
      virtual closer redeem(cell_info * of_cell) = 0;
    };

    struct static_value : i_value
    {
      void assign(cell_info * cell) override
      {
        traits::inner::assign_static(this, cell);
      }

      i_value::closer redeem(cell_info *) override { return & traits::simple_closer; }
    };

    template <typename Type, typename Derived>
    struct is_placed : public i_value
    {
      Type  value;

      is_placed(Type param) : value{ param } {}

      void assign(cell_info * to_cell) override
      { traits::inner::assign_placed(static_cast<Derived *>(this), to_cell); }

      i_value::closer redeem(cell_info *) override { return & traits::simple_closer; }
    };

    template <typename Derived>
    struct ref_counted : i_value
    {
      integer count{ 0 };

      void assign(cell_info * to_cell) override { to_cell->keep.handle = this; ++count; }

      i_value::closer redeem(cell_info *) override
      { return ( (--count) ? ( & traits::delete_function<cell_info> ) : nullptr ); }
    };


    namespace values {

      struct type : public static_value, public type_info, i_restriction
      {
        type(type_info param) : type_info{ std::move(param) } {}

        type_pointer get_type(system_data * panel) const override;

        bool match(const type * tp) const override
        {
          return (tp == this);
        }


        static constexpr auto index = &type_info::name;

        struct Less
        {
          bool operator () (literal lt, literal rt) const
          {
            return (lt->second.value < rt->second.value);
          }
        };

        static inline const auto auto_increment{ & type_info::position };
      };

    }

    struct system_data
    {
      struct params
      {
        log_system::log_pointer log;
        values::module_config * mod_global;

        dictionary                  all_names;
        static_table<values::type>  types_nest;
      };

      params p;
      t_counter   type_counter{ 1 };

      type_pointer t_category { inner::add_system_type(p, "$cat"    , -1) };
      type_pointer t_none     { inner::add_system_type(p, "$none"   , 0) };
      type_pointer t_type     { inner::add_system_type(p, "$type"   , type_counter() ) };
      type_pointer t_boolean  { inner::add_system_type(p, "$bool"   , type_counter() ) };
      type_pointer t_int      { inner::add_system_type(p, "$int"    , type_counter() ) };
      type_pointer t_counter  { inner::add_system_type(p, "$cnt"    , type_counter() ) };
      type_pointer t_text     { inner::add_system_type(p, "$text"   , type_counter() ) };
      type_pointer t_array    { inner::add_system_type(p, "$array"  , type_counter() ) };

      type_pointer t_module   { inner::add_system_type(p, "$module" , type_counter() ) };

      struct inner
      {
        static literal add_unique_name(
          log_system::log_pointer log,
          dictionary & dict, t_string name,
          std::source_location loc = std::source_location::current()
        )
        {
          typename dictionary::insert_result r = dict.add( std::move(name) );

          if( r.was_there )
          {
            log->append(& log_messages::startup_messages::sys_type_name_not_unique, loc);
            throw log_messages::startup_messages::sys_type_name_not_unique;
          }

          return r.get_pointer();
        }

        static type_pointer add_system_type(
          params & p, t_string name, integer type_index,
          std::source_location loc = std::source_location::current()
        )
        {
          // *
          return p.types_nest.append_row<type_info>( inner::add_unique_name(p.log, p.all_names, name, loc), type_index );
          return nullptr;
        }
      };
    };

    namespace values {

      type_pointer type::get_type(system_data * panel) const
      { return panel->t_type; }

      struct category : values::i_restriction, static_value
      {
        type_pointer get_type(system_data * panel) const override
        { return panel->t_category; }
      };

      struct none_type : public i_value
      {
        type_pointer get_type(system_data * panel) const override
        { return panel->t_none; }

        void assign(cell_info * to_cell) override
        {
          traits::inner::assign_static(this, to_cell);
        }

        void closer_none(cell_info *) {}

        i_value::closer redeem(cell_info *) override { return nullptr; }
      };

      struct none
      {
        static inline none_type value;
      };

      struct bool_value : public is_placed<bool, bool_value>
      {
        using base = is_placed<bool, bool_value>;
        using base::base;

        type_pointer get_type(system_data * panel) const override
        { return panel->t_boolean; }
      };

      struct int_value : is_placed<integer, int_value>
      {
        using base = is_placed<integer, int_value>;
        using base::base;

        type_pointer get_type(system_data * panel) const override
        { return panel->t_int; }
      };

      struct counter_int : i_value
      {
        std::intptr_t value;

        counter_int(std::intptr_t p) : value{ p }
        {}

        void assign(cell_info * to_cell) override;
        i_value::closer redeem(cell_info * of_cell) override;

        type_pointer get_type(system_data * panel) const override
        { return panel->t_counter; }
      };

      struct text_value : ref_counted<text_value>
      {
        t_string  value;
        text_value(t_string p) : value{ p } {}

        type_pointer get_type(system_data * panel) const override
        { return panel->t_text; }
      };

    };

  } /// system_types

    namespace execution::info {

      struct cell_type; using cell_pointer = cell_type *;

    }

  struct cell_info
  {
    lib::keep<
      system_types::i_value,
      system_types::values::bool_value,
      system_types::values::int_value,
      system_types::values::counter_int
    >  keep;
  };

  namespace system_types::values {

    void counter_int::assign(cell_info * to_cell)
    {
      to_cell->keep.handle = new( to_cell->keep.get() ) int_value{ value };
    }

    i_value::closer counter_int::redeem(cell_info * of_cell)
    {
      of_cell->keep.handle = & none::value;
      return nullptr;
    }

  }

  namespace execution::info {

    using point_type = std::set<cell_pointer>;
    using junction_type = std::map<point_type *, integer>;


    struct cell_type : ksi_traits::no_copyable, cell_info
    {
      static void delete_cell(cell_type * h) { delete h; }
      using close_function = decltype(& delete_cell);

      junction_type   junction;
      close_function  fn_close{ & delete_cell };

      cell_type(system_types::i_value * initial)
      {
        this->keep.handle = initial;
      }

      bool let(cell_type * other_cell)
      {
        traits::close_cell_value(this);
        other_cell->keep.handle->assign(this);
        return true;
      }
    };


    struct cell_holder :
      ksi_traits::no_copyable,
      ksi_traits::no_copy_assignable
    {
      cell_type * h;

      cell_holder(cell_type * cell_handle) : h{cell_handle} {}

      cell_holder() : h{ nullptr }
      {
        h = new cell_type{ & system_types::values::none::value };
      }

      [[nodiscard]] cell_type * release()
      {
        return std::exchange(this->h, nullptr);
      }

      cell_holder(cell_holder && other) : h{ nullptr }
      {
        this->h = other.release();
      }

      void operator = (cell_holder && other)
      {
        close();
        this->h = other.release();
      }

      ~cell_holder() { close(); }

    private:
      void close()
      {
        if( this->h != nullptr && this->h->junction.empty() )
        {
          traits::close_cell_value(this->h);
          cell_type::close_function fn = this->h->fn_close;
          fn( std::exchange(this->h, nullptr) );
        }
      }
    };


    struct slot {
      cell_type * handle{ nullptr };
      point_type       * _ptr;

      slot( point_type * hp ) : _ptr{ hp } { }

      void accept(cell_type * o)
      {
        auto [it, added] = o->junction.try_emplace(_ptr, 1);
        if ( added ) { return; }
        ++it->second;
      }

      bool uncell()
      {
        junction_type::iterator it = handle->junction.find( _ptr );
        if( handle->junction.end() == it ) { throw "Unable to find point in cell's junction"sv; }

        if( it->second > 1 ) { --it->second; }
        else { handle->junction.erase(it); }

        return handle->junction.empty();
      }
    };


    [[nodiscard]]
    cell_type * connect_pointed(cell_type * with, point_type & point)
    {
      point.insert(with);
      return with;
    }


    bool disjoin_pointed(cell_type * the_cell, point_type & from_point)
    {
      if( from_point.erase(the_cell) == 0 ) { throw "Can't disjoin the cell from point"sv; }
      return from_point.empty();
    }


  }
  namespace system_types {


    template <typename Derived, typename Data>
    struct pointed_value : i_value
    {
      std::unique_ptr<Data>         data;

      template <typename T>
      pointed_value(T param) : data{ std::make_unique<Data>(param) }
      {}

      void assign(cell_info * to_cell) override
      {
        execution::info::connect_pointed(static_cast<execution::info::cell_type *>(to_cell), data->point)->keep.handle = this;
      }

      i_value::closer redeem(cell_info * of_cell) override
      {
        if( execution::info::disjoin_pointed(static_cast<execution::info::cell_type *>(of_cell), data->point) )
        {
          std::list<execution::info::cell_holder> box;
          data->sweep(box);
          return & traits::delete_function<cell_info>;
        }
        return nullptr;
      }
    };


  }
  namespace system_types::values {

    struct array : public pointed_value<
      array,
      traits::array_impl<
        execution::info::slot, restriction_type
      , execution::info::point_type
      >
    >
    {
      using base_type = pointed_value<
        array,
        traits::array_impl<
          execution::info::slot, restriction_type
        , execution::info::point_type
        >
      >;
      using base_type::base_type;

      type_pointer get_type(system_data * panel) const
      { return panel->t_array; }
    };

    struct module_config : public static_value
    {
      type_pointer get_type(system_data * panel) const
      { return panel->t_module; }
    };

  }


  namespace traits {

    template <typename Value, typename To>
    inline void inner::assign_placed(Value * from, To * cell)
    {
      cell->keep.handle = new( cell->keep.get() ) Value{ from->value };
    }

    template <typename Value, typename To>
    inline void inner::assign_static(Value * from, To * cell)
    {
      cell->keep.handle = from;
    }

    inline void simple_closer(cell_info * cell)
    {
      cell->keep.handle = & system_types::values::none::value;
    }

    template <typename T>
    inline void delete_function(T * p)
    { delete std::exchange(p->keep.handle, & system_types::values::none::value); }

  }


} // ns
