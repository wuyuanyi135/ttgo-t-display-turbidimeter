//
// Created by wuyua on 4/20/2021.
//

#ifndef TURBIDITY_LED_CONTROL_H
#define TURBIDITY_LED_CONTROL_H

class led_control {
private:
  const int LED_PIN = 27;
  bool state = false;
public:
  void begin() const;
  void on() const;
  void off() const;
  void toggle();
};

#endif // TURBIDITY_LED_CONTROL_H
