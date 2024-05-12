#pragma once

#include "aliases.hpp"

#include <set>
#include <map>
#include <vector>
#include <forward_list>
#include <initializer_list>

#include <memory>
#include <ranges>

#include "lib/exception.hpp"
#include "lib/counter.hpp"
#include "lib/keep.hpp"

namespace ksi::interpreter
{
  using namespace std::literals::string_view_literals;
}
namespace ksi::interpreter::type_system
{

  namespace clearance
  {
    struct box_type;
  }
  namespace brick
  {
    struct cell_type;
    struct point_type;
  }
  namespace info
  {
    struct static_data;
  }
  namespace hints
  {
    struct category;
    struct type;

    using type_pointer = const type *;
    using cat_pointer = const category *;
  }

  struct security
  {
    std::set<brick::cell_type *>  visited_cells;
    std::set<brick::point_type *> visited_points;

    enum class option { rooted, tangled, empty };
    option check(brick::point_type & point);

    bool is_rooted(brick::cell_type * cell);
  };

  namespace base
  {

    template <typename Derived>
    struct dbg
    {
      static inline int cnt_made{ 0 }, cnt_out{ 0 };

      dbg() { ++cnt_made; }
      ~dbg() { ++cnt_out; }
    };

    struct i_value
    {
      static void out_none(i_value * value, brick::cell_type * cell, clearance::box_type & box) {}

      using into_result = void;
      using away_result = decltype( &out_none );
      using type_result = hints::type_pointer;

      virtual into_result  into(brick::cell_type * cell) = 0;
      virtual away_result  away() = 0;
      virtual type_result  get_type(info::static_data & sd) const = 0;
    };

    struct allocated : i_value
    {
      away_result fn;

      virtual away_result away() override
      { return this->fn; }

      virtual ~allocated() = default;
    };

    template <typename Data>
    struct pointed : allocated
    {
      static void out(i_value * value, brick::cell_type * cell, clearance::box_type & box);

      sys::unique<Data>  data{ std::make_unique<Data>() };

      pointed() { this->fn = &out; }

      virtual into_result into(brick::cell_type * cell) override;
    };

    struct counted : allocated
    {
      static void out(i_value * value, brick::cell_type * cell, clearance::box_type & box);

      sys::integer  ref_count{ 0 };

      counted() { this->fn = &out; }

      virtual into_result into(brick::cell_type * cell) override;
    };

    template <typename Bind, typename Derived>
    struct inplaced : i_value
    {
      using value_type = typename Bind::stored_type;

      value_type value;

      inplaced(value_type arg) : value{ arg } {}

      virtual into_result into(brick::cell_type * cell) override;

      virtual away_result away() override { return &out_none; }

      virtual type_result get_type(info::static_data & sd) const override;
    };

    struct staticly : i_value
    {
      virtual into_result into(brick::cell_type * cell) override;

      virtual away_result away() override { return &out_none; }
    };

  }
  namespace meta
  {

    struct meta_information
    {
      sys::literal  name;
      std::size_t   id;

      static inline const auto auto_increment{ &meta_information::id };

      using Less = detail::literal_less;
    };

  }
  namespace hints
  {

    struct i_hint
    {
      virtual bool match(type_pointer tp) = 0;
    };

    struct type : base::staticly, i_hint, meta::meta_information
    {
      std::set<category const *> relate_to_cats;

      type(meta::meta_information inf) : meta::meta_information{ inf } {}

      virtual type_result get_type(info::static_data & sd) const override;

      bool match(type_pointer tp) override { return (tp == this); }
    };

    struct category : base::staticly, i_hint, meta::meta_information
    {
      std::set<category *> insists_of;
      std::set<category *> insists_of_directly;

      category(meta::meta_information inf) : meta::meta_information{ inf } {}

      bool match(type_pointer tp) override { return tp->relate_to_cats.contains(this); }

      virtual type_result get_type(info::static_data & sd) const override;

      void sub_cat(category * other)
      {
        other->insists_of.insert(this);
        other->insists_of_directly.insert(this);
        other->insists_of.insert_range(this->insists_of);
      }
    };

  }
  namespace info
  {

    struct bind
    {
      template <typename Value, auto Member>
      struct to_system_type
      {
        using stored_type = Value;

        static constexpr auto pointer_to_member = Member;
      };
    };

    struct internal
    {
      static void cat_to_types(hints::cat_pointer cat, std::initializer_list<hints::type *> lst_types)
      {
        for( hints::type * tp : lst_types )
        {
          tp->relate_to_cats.insert(cat);
        }
      }

      struct params
      {
        sys::dictionary                     * dict;
        sys::static_table<hints::type>      type_table;
        sys::static_table<hints::category>  category_table;
      };

      static  hints::cat_pointer  reg_cat(params & p, sys::sview name)
      {
        return p.category_table.append_row<meta::meta_information>(
          p.dict->add(sys::string{name}).pointer
        ).result;
      }

      static  hints::type_pointer  reg_type(params & p, sys::sview name, std::initializer_list<hints::cat_pointer> cats)
      {
        hints::type * tp = p.type_table.append_row<meta::meta_information>(
          p.dict->add(sys::string{name}).pointer
        ).result;
        tp->relate_to_cats.insert_range(cats);
        return tp;
      }
    };

    struct static_data
    {
      internal::params r;

      hints::cat_pointer
        c_any     = internal::reg_cat(r, "_any"sv)
      ;

      hints::type_pointer
        t_cat     = internal::reg_type(r, "$cat"sv    , {}),
        t_type    = internal::reg_type(r, "$type"sv   , {c_any}),
        t_none    = internal::reg_type(r, "$none"sv   , {c_any}),
        t_bool    = internal::reg_type(r, "$bool"sv   , {c_any}),
        t_int     = internal::reg_type(r, "$int"sv    , {c_any}),
        t_literal = internal::reg_type(r, "$literal"sv, {c_any}),
        t_text    = internal::reg_type(r, "$text"sv   , {c_any}),
        t_array   = internal::reg_type(r, "$array"sv  , {c_any}),
        t_sequence= internal::reg_type(r, "$seq"sv    , {c_any})
      ;

      using type_bool = bind::to_system_type<bool, &static_data::t_bool>;
      using type_int = bind::to_system_type<sys::integer, &static_data::t_int>;
      using type_literal = bind::to_system_type<sys::literal, &static_data::t_literal>;
    };

  }
  namespace values
  {

    struct special
    {
      struct none_type : base::i_value
      {
        virtual into_result into(brick::cell_type * cell) override;

        virtual away_result away() override
        { return &out_none; }

        virtual type_result get_type(info::static_data & sd) const override { return sd.t_none; }
      };

      static inline none_type none{};
    };

    struct bool_value : base::inplaced<info::static_data::type_bool, bool_value>
    {
      using base::inplaced<info::static_data::type_bool, bool_value>::inplaced;
    };

    struct int_value : base::inplaced<info::static_data::type_int, int_value>
    {
      using base::inplaced<info::static_data::type_int, int_value>::inplaced;

      ~int_value() { std::print("~int_value: {}\n", this->value); }
    };

    struct literal_value : base::inplaced<info::static_data::type_literal, literal_value>
    {
      using base::inplaced<info::static_data::type_literal, literal_value>::inplaced;

      ~literal_value() { std::print("~literal_value: {}\n", this->value->first); }
    };

    struct text_value : base::counted
    {
      sys::string  value;

      text_value(sys::string arg) : value{ std::move(arg) }
      {}

      virtual type_result get_type(info::static_data & sd) const override { return sd.t_text; }

      ~text_value() { std::print(" ~text: {}\n", value); }
    };

  }
  namespace brick
  {

    bool flag_join{ false };
    bool flag_point{ false };
    struct point_type;

    struct junction_type
    {
      using map_type = std::map<point_type *, sys::integer>;

      map_type map;

      void join(point_type * point)
      {
        if( flag_join ) { msg("Test junction before map try_emplace"); } //dbg:
        if( auto [it, added] = map.try_emplace(point, 1); !added )
        { ++it->second; std::print(" [linked in]\n"); }
      }

      void disjoin(point_type * point)
      {
        typename map_type::iterator it{ map.find(point) };
        if( map.end() == it ) { error_msg("Unable to find point in cell's junction"); return; }

        if( it->second > 1 ) { --it->second; }
        else{ map.erase(it); }
      }
    };

    struct cell_type : base::dbg<cell_type>
    {
      static void finish(cell_type * h, bool) { delete h; }
      static void unbind(cell_type * h, bool reset_only);

      using keep_type = lib::keep<base::i_value, values::bool_value, values::literal_value>;

      keep_type         keep;
      junction_type     junction;
      decltype(&finish) fn{ &unbind };

      cell_type() : keep{ .handle = &values::special::none }
      {}

      cell_type(cell_type const & other)
      {
        assign(other.keep.handle);
      }

      void assign(base::i_value * value)
      {
        value->into(this);
      }

      template <typename Box>
      void complete(Box * box)
      {
        base::i_value::away_result function = keep.handle->away();
        function(keep.handle, this, *box);
      }

      void done()
      {
        this->fn(this, true);
      }

      struct deleter
      {
        void operator ()(cell_type * h) const { h->fn(h, false); }

        deleter() = default;
        deleter(std::default_delete<cell_type> &&) noexcept {}
      };
    };

    using cell_ptr = std::unique_ptr<brick::cell_type, brick::cell_type::deleter>;

    struct holder
    {
      cell_type * h;

      holder(const holder &) = delete;
      holder & operator = (const holder &) = delete;

      holder() : holder{ &values::special::none }
      {}

      holder( base::i_value * value ) : h{ nullptr }
      {
        cell_ptr cp = std::make_unique<cell_type>();
        cp->assign( value );
        h = cp.release();
      }

      ~holder()
      {
        if( h != nullptr ) { h->fn(h, false); h = nullptr; }
      }

      void make_empty()
      {
        h = nullptr;
      }
    };

    struct slot
    {
      point_type *  point;
      cell_type *   handle;

      slot(point_type * pt) : point{ pt }, handle{ new cell_type() }
      { error_msg("{here}"); }

      slot(const slot & other) : point{ other.point }
      {
        handle = new cell_type{ *other.handle };
        handle->junction.join(point);
        { error_msg("{here too}"); }
      }

      slot(point_type * pt, holder & hold) : point{ pt }
      {
        handle = new cell_type{ *hold.h };
        handle->junction.join(point);
        { error_msg("{here again}"); }
      }

      void uncell()
      {
        handle->junction.disjoin(point);
      }

      base::i_value * get_value() { return handle->keep.handle; }
    };

    struct point_type
    {
      using set_type = std::set<cell_type *>;

      set_type set;

      void connect_with(cell_type * cell)
      {
        if( flag_point ) { msg("Test point before set insert"); } //dbg:
        set.insert(cell);
      }

      void disconnect_from(cell_type * cell)
      {
        if( set.erase(cell) == 0 )
        { error_msg("Can't disconnect the cell from point"); }
      }

      void disconnect()
      {
        for( cell_type * cell : set )
        {
          cell->keep.handle = &values::special::none;
        }
        set.clear();
      }
    };

  }
  namespace clearance
  {

    struct box_type
    {
      std::forward_list<brick::cell_ptr>  cells;

      void add_cell(brick::cell_type * cell)
      {
        cells.emplace_front( cell );
        cell->complete(this);
        cell->done();
      }
    };

    template <typename T>
    struct impl_deleter
    {
      static void close(T * h) { delete h; }

      decltype( &close ) fn{ &close };

      impl_deleter() = default;
      impl_deleter(std::default_delete<T>) {}

      void operator () (T * h)
      {
        box_type box;
        h->sweep_cells(box);

        this->fn(h);
      }
    };

    template <typename Impl>
    using unique = std::unique_ptr< Impl, impl_deleter<Impl> >;

  }
  namespace impl
  {

    struct impl_array
    {
      brick::point_type         point;
      std::vector<brick::slot>  elements;

      impl_array() = default;
      impl_array(std::size_t amount_to_reserve)
      {
        elements.reserve( amount_to_reserve );
      }

      void sweep_cells(clearance::box_type & box)
      {
        for( brick::slot & slot : elements | std::views::reverse )
        {
          slot.uncell();
          if( security walker; walker.is_rooted(slot.handle) ) { continue; }
          box.add_cell(slot.handle);
        }
      }

      void append(brick::holder && hold)
      {
        elements.emplace_back(&point, hold);
        hold.make_empty();
      }
    };

  }
  namespace collections
  {

    struct array : base::pointed<impl::impl_array>, base::dbg<array>
    {
      array() = default;
      array(std::size_t reserve)
      {
        this->data->elements.reserve( reserve );
      }

      virtual type_result get_type(info::static_data & sd) const override { return sd.t_array; }

      ~array() { std::print("~array\n"); }
    };

  }
  struct hidden
  {
    struct inner
    {
      template <typename T>
      operator T *& () const
      {
        return secret<T>;
      }

    private:
      template <typename T>
      static inline T * secret;
    };

    static inline const inner value;
  };
  namespace creation
  {

    template <typename Value, typename Type>
    struct maker
    {
      brick::holder operator () (Type value) const
      {
        Value v{ value };
        return { &v };
      }
    };


    constexpr maker<values::bool_value, bool> make_bool{};
    constexpr maker<values::literal_value, sys::literal> make_literal{};

    inline brick::holder make_text(sys::string value, values::text_value *& v = hidden::value)
    {
      return brick::holder{ v = new values::text_value{std::move(value)} };
    }

    inline brick::holder make_array(std::size_t reserve, collections::array *& v = hidden::value)
    {
      return brick::holder{ v = new collections::array{reserve} };
    }

  }

  // category

  auto hints::category::get_type(info::static_data & sd) const -> type_result
  {
    return sd.t_cat;
  }

  // type

  auto hints::type::get_type(info::static_data & sd) const -> type_result
  {
    return sd.t_type;
  }

  // inplaced

  template <typename Bind, typename Derived>
  inline auto base::inplaced<Bind, Derived>::get_type(info::static_data & sd) const -> type_result { return sd.*(Bind::pointer_to_member); }

  template <typename Bind, typename Derived>
  inline base::i_value::into_result base::inplaced<Bind, Derived>::into(brick::cell_type * cell)
  {
    cell->keep.handle = (
      new (&cell->keep.inner.data) Derived{ this->value }
    );
  }

  // staticly

  inline base::i_value::into_result base::staticly::into(brick::cell_type * cell)
  {
    cell->keep.handle = this;
  }

  // none

  inline base::i_value::into_result values::special::none_type::into(brick::cell_type * cell)
  {
    cell->keep.handle = &none;
  }

  // cell_type

  inline void brick::cell_type::unbind(cell_type * h, bool reset_only)
  {
    if( reset_only ) { h->fn = &finish; return; }

    try
    {
      if( security guard; guard.is_rooted(h) ) { return; }
    }
    catch( ... )
    {
      lib::exception::save();
      //unsure_cell
      return;
    }

    try
    {
      clearance::box_type box;
      h->complete( &box );
      delete h;
    }
    catch( ... )
    {
      lib::exception::save();
      //not_unbound
    }
  }

  // counted

  inline base::i_value::into_result base::counted::into(brick::cell_type * cell)
  {
    cell->keep.handle = this;
    ++this->ref_count;
  }

  inline void base::counted::out(i_value * value, brick::cell_type * cell, clearance::box_type & box)
  {
    counted * h = static_cast<counted *>(value);

    if( h->ref_count > 1 ) { --h->ref_count; return; }

    delete h;
  }

  // pointed

  template <typename Data>
  inline base::i_value::into_result base::pointed<Data>::into(brick::cell_type * cell)
  {
    cell->keep.handle = this;
    data->point.connect_with(cell);
  }

  template <typename Data>
  inline void base::pointed<Data>::out(i_value * value, brick::cell_type * cell, clearance::box_type & box)
  {
    pointed * h = static_cast<pointed *>(value);

    h->data->point.disconnect_from(cell);
    switch( security watcher; watcher.check(h->data->point) )
    {
      case security::option::rooted : return;
      case security::option::tangled : { h->data->point.disconnect(); break; }
      case security::option::empty : break;
    }

    h->data->sweep_cells(box);
    delete h;
  }

  // security

  inline bool security::is_rooted(brick::cell_type * cell)
  {
    for( auto [point, cnt] : cell->junction.map )
    if( this->check( *point ) == option::rooted )
    { return true; }

    return false;
  }

  inline security::option security::check(brick::point_type & point)
  {
    if( point.set.empty() ) { return option::empty; }

    using iterator = typename brick::point_type::set_type::iterator;
    using pair_type = std::pair<iterator, iterator>;

    std::forward_list<pair_type> stack;
    stack.emplace_front( point.set.begin(), point.set.end() );

    do
    {
      for( pair_type & pair{ stack.front() }; pair.first != pair.second; ++pair.first )
      {
        brick::cell_type * cell = *pair.first;
        if( cell->junction.map.empty() ) { return option::rooted; }
        if( visited_cells.contains(cell) ) { continue; }
        visited_cells.insert(cell);

        for( auto [pt, cnt] : cell->junction.map )
        {
          if( pt->set.empty() ) { return option::rooted; }
          if( ! visited_points.contains(pt) )
          {
            visited_points.insert(pt);
            stack.emplace_front( pt->set.begin(), pt->set.end() );
          }
        }
      }

      stack.pop_front();
    }
    while( ! stack.empty() );

    return option::tangled;
  }

}
