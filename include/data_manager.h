//
// Created by wuyua on 4/20/2021.
//

#ifndef TURBIDITY_DATA_MANAGER_H
#define TURBIDITY_DATA_MANAGER_H
#include "vector"
#include "Arduino.h"

static const char *data_file_path = "/data.dat";
static const uint32_t AUTO_SYNC_MS = 60000;

class data_manager {
private:
  std::vector<uint16_t> data;
  uint32_t last_sync = 0;
public:
  bool recording = false;

public:
  void begin();
  void update();
  void feed(uint16_t v);
  void sync();
  void erase();
  void start_record();
  void stop_record();
  size_t get_size();
  String get_size_string();
};

#endif // TURBIDITY_DATA_MANAGER_H
