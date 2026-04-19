#ifndef ESPNOW_SENDER_H
#define ESPNOW_SENDER_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Cấu trúc dữ liệu gửi
typedef struct {
  char command[10];
} DataSend;

// ❗ QUAN TRỌNG: Thay MAC bằng MAC của ESP32 trên XE
extern uint8_t receiverMac[];

void initESPNowSender();
void sendCommand(String cmd);

#endif