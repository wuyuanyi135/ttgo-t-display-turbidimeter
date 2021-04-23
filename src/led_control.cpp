//
// Created by wuyua on 4/20/2021.
//
#include "led_control.h"
#include "Arduino.h"
void led_control::begin() const { pinMode(LED_PIN, OUTPUT); }
void led_control::on() const { digitalWrite(LED_PIN, HIGH); }
void led_control::off() const { digitalWrite(LED_PIN, LOW); }
void led_control::toggle() {
  if (!state) {
    on();
  } else {
    off();
  }
  state = !state;
}
