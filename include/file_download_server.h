//
// Created by wuyua on 4/20/2021.
//

#ifndef TURBIDITY_FILE_DOWNLOAD_SERVER_H
#define TURBIDITY_FILE_DOWNLOAD_SERVER_H

#include "data_manager.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

static const char* ssid     = "Turbidy-AP";
static const char* password = "123456789";

static AsyncWebServer http_server(80);

class file_download_server {
public:
  data_manager &dm;

  explicit file_download_server(data_manager& dm);
  void begin();
  void update();
};

#endif // TURBIDITY_FILE_DOWNLOAD_SERVER_H
