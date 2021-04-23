#include "data_manager.h"
#include "data_source.h"
#include "file_download_server.h"
#include "led_control.h"
#include "ui.h"

data_source ds;
led_control lc;
data_manager dm;
file_download_server srv(dm);
ui s(lc, ds, dm);

void setup() {
  Serial.begin(115200);
  dm.begin();
  ds.begin();
  lc.begin();
  s.begin();

  srv.begin();
}

void loop() {
  ds.update();
  s.update();
  dm.update();
  srv.update();
}