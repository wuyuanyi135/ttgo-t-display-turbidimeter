//
// Created by wuyua on 4/20/2021.
//

#include "file_download_server.h"
#include "SPIFFS.h"
#include "data_manager.h"
void file_download_server::begin() {
  WiFi.softAP(ssid, password);

  http_server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
    dm.sync();
    request->send(SPIFFS, data_file_path, "application/x-binary", true);

  });

  http_server.begin();
}
void file_download_server::update() {}
file_download_server::file_download_server(data_manager &dm) : dm(dm) {}
