//
// Created by wuyua on 4/20/2021.
//

#ifndef TURBIDITY_UI_H
#define TURBIDITY_UI_H

#include "OneButton.h"
#include "data_manager.h"
#include "data_source.h"
#include "led_control.h"
#include <TFT_eSPI.h>
#include <lvgl.h>
static const int VDIV_COUNT = 11; // hours
static const int POINTS_ON_SCREEN = 120;
static const int SKIP_EVERY = 6*60; // every 6 minutes
static const char* XTICKS = "12\n11\n10\n9\n8\n7\n6\n5\n4\n3\n2\n1\n0";
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];

static TFT_eSPI tft = TFT_eSPI();

/*Display flushing*/
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area,
                   lv_color_t *color_p);

class ui {
public:
  OneButton btn_up{35};
  OneButton btn_down{0};

  led_control &lc;
  data_source &ds;
  data_manager &dm;
  lv_obj_t *chart;
  lv_chart_series_t *series;
  lv_obj_t *label_now;
  lv_obj_t *label_file_size;
  lv_obj_t *label_recording;
  std::queue<uint16_t> measurement;
  int skip_cnt = SKIP_EVERY+1; // first data
public:
  ui(led_control &lc, data_source &ds, data_manager &dm);
  void begin();
  void update();
  void initialize_ui();
  void toggle_recording();
  void update_size();
};

#endif // TURBIDITY_UI_H
