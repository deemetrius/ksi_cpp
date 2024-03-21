#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ksi_lib/dict.hpp"
#include <utility>

namespace ui {


  struct theme;


  struct i_control
  {
    virtual ~i_control() = default;

    virtual void draw(theme & t) = 0;
  };

  using i_control_pointer = i_control *;


  struct i_theme
  {
    virtual void draw(i_control * ctl) = 0;
  };

  template <typename Control, typename Theme>
  struct theme_instance : public i_theme
  {
    using type = Theme::template instance<Control>;

    type sub_theme;

    void draw(i_control * ctl) override
    {
      sub_theme.draw( static_cast<Control *>(ctl) );
    }
  };


  struct theme_instance_default : public i_theme
  {
    void draw(i_control * ctl) override
    {}
  };


  template <typename First_Control, typename ... Rest_Controls>
  struct supported
  {
    template <typename Theme, typename Container>
    static void fill(Container & cont)
    {
      using theme_inst_type = theme_instance<First_Control, Theme>;

      cont[First_Control::type_id] = std::make_shared<theme_inst_type>();

      if constexpr( sizeof...(Rest_Controls) > 0 )
      {
        supported<Rest_Controls ...>::template fill<Theme>(cont);
      }
    }
  };

  struct theme
  {
    using string_type = std::string;
    using dict_type = ksi::lib::dict<string_type>;
    using pointer = std::shared_ptr<i_theme>;
    using instances_type = std::vector<pointer>;

    static inline dict_type control_types{};

    instances_type instances;

    template <typename Theme>
    theme(std::in_place_type_t<Theme>)
    {
      instances.resize( control_types.set.size() );
      Theme::supported_controls::template fill<Theme>(this->instances);
      for( pointer & it : this->instances )
      if( not it )
      {
        it = std::make_shared<theme_instance_default>();
      }
    }
  };


  struct base_control
  {};


  template <typename Control>
  struct is_control : public i_control
  {
    //using self_params = typename Control::Params;

    static inline const std::size_t type_id = theme::control_types.add(Control::type_name).it->id;

    //self_params params;

    //is_control(self_params p) : p{std::move(p)} {}

    void draw(theme & t) override { t.instances[type_id]->draw(this); }
  };


  struct Button : public is_control<Button>
  {
    static inline theme::string_type type_name = "ui.Button";

    struct Params
    {
      theme::string_type caption;
    };
  };


  struct Label : public is_control<Button>
  {
    static inline theme::string_type type_name = "ui.Label";

    struct Params
    {
      theme::string_type  text;
      i_control_pointer   for_control{ nullptr };
    };
  };


  struct first_theme
  {
    using supported_controls = supported<Button, Label>;

    template <typename Control>
    struct instance
    {
      void draw(Control * ctl) {}
    };
  };


} // ns
