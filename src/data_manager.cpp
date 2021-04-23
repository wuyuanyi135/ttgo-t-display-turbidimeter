//
// Created by wuyua on 4/20/2021.
//

#include "data_manager.h"
#include "Arduino.h"
#include "SPIFFS.h"

void data_manager::begin() { SPIFFS.begin(true); }
void data_manager::feed(uint16_t v) {
  if (!recording)
    return;
  data.push_back(v);
}
void data_manager::update() {

  auto now = millis();
  if (now >= last_sync + AUTO_SYNC_MS) {
    last_sync = now;
    sync();
  }
}
void data_manager::sync() {
  auto file = SPIFFS.open(data_file_path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to append file");
  }
  for (int i = 0; i < data.size(); ++i) {
    auto d = data[i];

    file.write((uint8_t *)&d, 2);
  }
  data.clear();
  file.close();
  Serial.println("Syncing");
}
void data_manager::erase() { SPIFFS.remove(data_file_path); }
void data_manager::start_record() {
  if (recording) {
    return;
  }
  recording = true;

  erase();
  auto file = SPIFFS.open(data_file_path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
  }

  file.close();
}
void data_manager::stop_record() {
  if (!recording) {
    return;
  }
  recording = false;
  sync();
}
size_t data_manager::get_size() {
  auto file = SPIFFS.open(data_file_path, FILE_READ);
  if (!file) {
    Serial.println("Failed to read file");
  }
  auto sz = file.size();
  file.close();

  return sz;
}
String data_manager::get_size_string() {
  auto sz = get_size();

  if (sz < 1024) {
    return String((int)sz) + " B";
  } else if (sz < 1024 * 1024) {
    return String((double)sz / 1024, 2) + "KB";
  }
}
