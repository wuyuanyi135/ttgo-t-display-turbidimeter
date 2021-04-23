//
// Created by wuyua on 4/20/2021.
//

#include "ui.h"
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

void toggle_lc(void *lc) { ((led_control *)lc)->toggle(); }
void toggle_record(void *userdata) {
  auto this_ = (ui *)userdata;
  this_->toggle_recording();
}
void ui::begin() {

  ds.register_queue(measurement);

  btn_up.attachClick(toggle_lc, (void *)&lc);
  btn_down.attachLongPressStop(toggle_record, (void *)this);

  lv_init();
  tft.init();
  tft.setRotation(1);

  lv_disp_buf_init(&disp_buf, buf, nullptr, LV_HOR_RES_MAX * 10);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LV_HOR_RES;
  disp_drv.ver_res = LV_VER_RES;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  initialize_ui();
}
void ui::update() {

  btn_up.tick();
  btn_down.tick();
  lv_task_handler();

  if (!measurement.empty()) {
    const auto v = measurement.front();

    lv_label_set_text_fmt(label_now, "%d", v);
    measurement.pop();

    if (skip_cnt ++ >= SKIP_EVERY) {
      skip_cnt = 0;
      lv_chart_set_next(chart, series, v);
    }
    dm.feed(v);
  }
}

ui::ui(led_control &lc, data_source &ds, data_manager &dm)
    : lc(lc), ds(ds), dm(dm) {}

void ui::initialize_ui() {

  lv_obj_t *cont;

  cont = lv_cont_create(lv_scr_act(), nullptr);
  lv_obj_set_size(cont, LV_HOR_RES_MAX, LV_VER_RES_MAX);
  lv_cont_set_layout(cont, LV_LAYOUT_COLUMN_MID);
  lv_obj_set_style_local_pad_all(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_margin_all(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT,
                                    0);
  lv_obj_set_style_local_pad_inner(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 1);

  chart = lv_chart_create(cont, nullptr);
  const int chart_height = 115;
  lv_obj_set_size(chart, LV_HOR_RES_MAX, chart_height);
  lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
  lv_chart_set_div_line_count(chart, 0, VDIV_COUNT);
  lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
  lv_chart_set_range(chart, 0, 4096);
  lv_chart_set_point_count(chart, POINTS_ON_SCREEN);
  lv_chart_set_x_tick_texts(chart, XTICKS, 0, LV_CHART_AXIS_DRAW_LAST_TICK);
  lv_chart_set_x_tick_length(chart, 5, 1);
  lv_obj_set_style_local_pad_bottom(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 25);
  lv_obj_set_style_local_margin_all(chart, LV_CHART_PART_BG, LV_STATE_DEFAULT,
                                    0);

  series = lv_chart_add_series(chart, LV_COLOR_RED);

  auto right_cont = lv_cont_create(cont, nullptr);
  lv_obj_set_size(right_cont, LV_HOR_RES_MAX, LV_VER_RES_MAX - chart_height);

  lv_cont_set_layout(right_cont, LV_LAYOUT_ROW_MID);
  lv_obj_set_style_local_pad_all(right_cont, LV_CONT_PART_MAIN,
                                 LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_margin_all(right_cont, LV_CONT_PART_MAIN,
                                    LV_STATE_DEFAULT, 0);

  label_recording = lv_label_create(right_cont, nullptr);
  lv_label_set_text(label_recording, "Not Recording");

  label_file_size = lv_label_create(right_cont, nullptr);
  lv_label_set_text(label_file_size, "-");
  update_size();
  lv_task_create(
      [](lv_task_t *task) {
        auto this_ = (ui *)task->user_data;
        this_->update_size();
      },
      10000, LV_TASK_PRIO_LOW, (void *)this);

  label_now = lv_label_create(right_cont, nullptr);
  lv_label_set_text(label_now, "-");

}

void ui::toggle_recording() {

  if (dm.recording) {
    dm.stop_record();
    lv_label_set_text(label_recording, "Not Recording");

  } else {
    dm.start_record();
    lv_label_set_text(label_recording, "Recording");
  }
}
void ui::update_size() {
  auto size = dm.get_size_string();
  lv_label_set_text(label_file_size, size.c_str());
}
