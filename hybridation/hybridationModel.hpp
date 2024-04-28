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
    halp::xy_pad_f32<"Naviguateur"> navigateur;
    halp::xy_pad_f32<"Blob 1"> blob1;
    halp::xy_pad_f32<"Blob 2"> blob2;
    halp::xy_pad_f32<"Blob 3"> blob3;
    halp::xy_pad_f32<"Blob 4"> blob4;
  } inputs;

  struct
  {
    halp::val_port<"out", float> out1;
    halp::val_port<"out", float> out2;
    halp::val_port<"out", float> out3;
    halp::val_port<"out", float> out4;

  } outputs;

  struct processor_to_ui
  {
    float x_input;
    float y_input;

    std::vector<float> blob1;
    std::vector<float> blob2;
    std::vector<float> blob3;
    std::vector<float> blob4;

  };

  std::function<void(processor_to_ui)> send_message;

  struct custom_anim
  {
    using item_type = custom_anim;
    static constexpr float width() { return 200.; }
    static constexpr float height() { return 200.; }

    float rect_x;
    float rect_y;

    std::vector<float> blob1 = {0.2, 0.8};
    std::vector<float> blob2 = {0.8, 0.8};
    std::vector<float> blob3 = {0.8, 0.2};
    std::vector<float> blob4 = {0.2, 0.2};



    void paint(avnd::painter auto ctx)
    {

      ctx.set_stroke_color({.r = 92, .g = 53, .b = 102, .a = 255});
      ctx.set_fill_color({173, 127, 168, 255});
      ctx.draw_circle(blob1[0]*width(), blob1[1]*height(), 15.);
      ctx.draw_circle(blob2[0]*width(), blob2[1]*height(), 15.);
      ctx.draw_circle(blob3[0]*width(), blob3[1]*height(), 15.);
      ctx.draw_circle(blob4[0]*width(), blob4[1]*height(), 15.);

      ctx.set_stroke_color({.r = 42, .g = 153, .b = 153, .a = 255});

      ctx.draw_rect(rect_x*width(), rect_y*height(), 10., 10.);

      ctx.fill();
      ctx.stroke();

      ctx.update();
    }

  };

  void operator()()
  {
    auto [ix,iy] = inputs.navigateur.value;

    auto [ax, ay] = inputs.blob1.value;
    auto [bx, by] = inputs.blob2.value;
    auto [cx, cy] = inputs.blob3.value;
    auto [dx, dy] = inputs.blob4.value;

    outputs.out1.value = fmax(1.0 - sqrt(pow(ix-ax,2.0) + pow(iy-ay,2.0)),0.);
    outputs.out2.value = fmax(1.0 - sqrt(pow(ix-bx,2.0) + pow(iy-by,2.0)),0.);
    outputs.out3.value = fmax(1.0 - sqrt(pow(ix-cx,2.0) + pow(iy-cy,2.0)),0.);
    outputs.out4.value = fmax(1.0 - sqrt(pow(ix-dx,2.0) + pow(iy-dy,2.0)),0.);

    send_message(processor_to_ui{.x_input = ix, .y_input = iy,
                                 .blob1 = {ax, ay}, .blob2 = {bx, by}, .blob3 = {cx, cy}, .blob4 = {dx,dy}});

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
        self.a_vbox.anim.blob1 = msg.blob1;
        self.a_vbox.anim.blob2 = msg.blob2;
        self.a_vbox.anim.blob3 = msg.blob3;
        self.a_vbox.anim.blob4 = msg.blob4;

      }

    };

    struct
    {
      halp_meta(layout, halp::layouts::hbox)
      halp_meta(name, "HBox")
      halp::item<&ins::navigateur> navigateur;
    } a_hbox;

    struct
    {
      halp_meta(layout, halp::layouts::grid)
      halp_meta(columns, 2)
      halp_meta(name, "Blobs")
      halp::item<&ins::blob1> blob1;
      halp::item<&ins::blob2> blob2;
      halp::item<&ins::blob3> blob3;
      halp::item<&ins::blob4> blob4;
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
