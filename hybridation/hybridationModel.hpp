#pragma once

#include <avnd/concepts/processor.hpp>

#include <avnd/concepts/painter.hpp>

#include <cmath>
#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/controls_fmt.hpp>
#include <halp/layout.hpp>
#include <halp/meta.hpp>
#include <QDebug>

namespace Example
{

struct hybridation
{
  halp_meta(name, "Mon objet")
  halp_meta(category, "Spatial")
  halp_meta(c_name, "mon_objet")
  halp_meta(author, "Jean-Michaël Celerier, Manuel Bolduc, Eduardo Meneses, Zachary L'heureux")
  halp_meta(uuid, "bcd82b30-cde3-47ba-b151-211cbde66a25")

  struct ins
  {
    halp::hslider_f32<"Pointeur"> x_input;
    halp::hslider_f32<"Pointeur2"> y_input;
    halp::xyz_spinboxes_f32<"Objet 1"> blob1;
  } inputs;

  struct
  {
    halp::val_port<"out", float> out;

  } outputs;

  struct processor_to_ui
  {
    float x_input;
    float y_input;
  };

  std::function<void(processor_to_ui)> send_message;

  struct custom_anim
  {
    using item_type = custom_anim;
    static constexpr float width() { return 200.; }
    static constexpr float height() { return 200.; }

    float rect_x;
    float rect_y;


    void paint(avnd::painter auto ctx)
    {

      ctx.set_stroke_color({.r = 92, .g = 53, .b = 102, .a = 255});
      ctx.set_fill_color({173, 127, 168, 255});
      ctx.draw_rect(rect_x*width(), rect_y*height(), 15., 15.);
      ctx.draw_rect(175., 175., 15., 15.);
      ctx.draw_rect(15., 175., 15., 15.);
      ctx.draw_rect(175., 15., 15., 15.);

      ctx.fill();
      ctx.stroke();

      ctx.update();
    }

  };

  void operator()()
  {
    auto ix = inputs.x_input.value;
    auto iy = inputs.y_input.value;

    auto [ax, ay, az] = inputs.blob1.value;
    outputs.out.value = fmax(1.0 - sqrt(pow(ix-ax,2.0) + pow(iy-ay,2.0))/az,0.);
    send_message(processor_to_ui{.x_input = ix, .y_input = iy});

  }

  struct ui
  {
    halp_meta(name, "Main")
    halp_meta(layout, halp::layouts::hbox)
    halp_meta(background, halp::colors::mid)

    struct bus
    {
      // 4. Receive a message on the UI thread from the processing thread
      static void process_message(ui& self, processor_to_ui msg)
      {
        qDebug() << msg.x_input << msg.y_input;
        self.a_vbox.anim.rect_x = msg.x_input;
        self.a_vbox.anim.rect_y = msg.y_input;

      }

    };

    struct
    {
      halp_meta(layout, halp::layouts::hbox)
      halp_meta(name, "HBox")
      halp::item<&ins::x_input> x_input;
      halp::item<&ins::y_input> y_input;
    } a_hbox;

    struct
    {
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(name, "VBox")
      halp::item<&ins::blob1> blob1;
    } b_vbox;

    struct
    {
      halp_meta(layout, halp::layouts::vbox)
      halp_meta(name, "dynamic_mapping")
      halp_meta(width, 300)
      halp_meta(height, 200)

      halp::custom_actions_item<custom_anim> anim{.x = 90, .y = -50};

    } a_vbox;
  };


};

}
