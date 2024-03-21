#pragma once

#include "ui_control.hpp"
#include <vector>

namespace ui {


  struct i_theme
  {
    virtual void draw(control_handle ctl) = 0;
  };

  template <typename Control, typename Theme>
  struct theme_instance : public i_theme
  {
    using type = Theme::template instance<Control>;

    type sub_theme;

    theme_instance(typename Theme::theme_config const & params) : sub_theme{ params }
    {}

    void draw(control_handle ctl) override
    {
      sub_theme.draw( static_cast<control<Control> *>(ctl) );
    }
  };


  struct theme_instance_default : public i_theme
  {
    void draw(control_handle ctl) override
    {}
  };


  template <typename First_Control, typename ... Rest_Controls>
  struct supported
  {
    template <typename Container, typename Theme>
    static void fill(Container & cont, Theme t, typename Theme::theme_config const & params)
    {
      using theme_inst_type = theme_instance<First_Control, Theme>;

      cont[control<First_Control>::self_type->id] = std::make_shared<theme_inst_type>(params);

      if constexpr( sizeof...(Rest_Controls) > 0 )
      {
        supported<Rest_Controls ...>::fill(cont, t, params);
      }
    }
  };

  struct theme
  {
    using pointer = std::shared_ptr<i_theme>;
    using instances_type = std::vector<pointer>;

    instances_type instances;

    template <typename Theme>
    theme(Theme t, typename Theme::theme_config const & params)
    {
      instances.resize( system::control_types.set.size() );
      Theme::supported_controls::fill(this->instances, t, params);
      for( pointer & it : this->instances )
      if( not it )
      {
        it = std::make_shared<theme_instance_default>();
      }
    }

    void draw(control_pointer ctl) { this->instances[ctl->type->id]->draw(ctl.get()); }
  };


} // ns
