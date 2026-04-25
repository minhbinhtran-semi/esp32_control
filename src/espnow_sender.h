#ifndef ESPNOW_SENDER_H
#define ESPNOW_SENDER_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  char cmd[10];
} DataSend;

extern uint8_t receiverMac[];

void initESPNowSender();
void sendCommand(String cmd);

#endif