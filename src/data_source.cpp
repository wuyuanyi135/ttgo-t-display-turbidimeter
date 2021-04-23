//
// Created by wuyua on 4/20/2021.
//
#include "data_source.h"
#include <cmath>

void data_source::begin() const {
  pinMode(ADC_PIN, INPUT);
  adcAttachPin(ADC_PIN);
}

void data_source::update() {
  auto current_time = millis();
  if (current_time > last_sample_time_ms + ADC_SAMPLE_DELAY_MS) {
    last_sample_time_ms = current_time;
    uint16_t value = analogRead(ADC_PIN);
    sample_buffer[sample_buffer_idx++] = value;


    if (sample_buffer_idx >= MEAN_SAMPLE) {
      uint32_t s = 0;
      for (int i = 0; i < sample_buffer_idx; ++i) {
        s += sample_buffer[i];
      }
      uint16_t mean = s / sample_buffer_idx;

      for (int i = 0; i < destination_queues.size(); ++i) {
        destination_queues[i]->push(mean);
      }
      sample_buffer_idx = 0;

    }
  }
}
void data_source::register_queue(std::queue<uint16_t> &q) {
  destination_queues.push_back(&q);
}
