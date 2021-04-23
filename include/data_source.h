//
// Created by wuyua on 4/20/2021.
//

#ifndef TURBIDITY_DATA_SOURCE_H
#define TURBIDITY_DATA_SOURCE_H
#include <array>
#include <queue>
#include "Arduino.h"

const int ADC_SAMPLE_DELAY_MS = 100;
const int ADC_PIN = 36;
const int MEAN_SAMPLE = 10;

class data_source {
private:
  std::array<uint16_t, MEAN_SAMPLE> sample_buffer;
  uint32_t sample_buffer_idx = 0;

  uint32_t last_sample_time_ms = 0;

  std::vector<std::queue<uint16_t>*> destination_queues;
public:
  void begin() const;
  void update();
  void register_queue(std::queue<uint16_t> &q);
};

#endif // TURBIDITY_DATA_SOURCE_H
