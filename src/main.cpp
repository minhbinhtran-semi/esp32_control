#include <Arduino.h>
#include "mpu6050.h"
#include "espnow_sender.h"

float gocX = 0, gocY = 0, tocDoXoayZ = 0;
String lastCmd = "";
unsigned long lastSendTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  initMPU6050();
  initESPNowSender();
  
  Serial.println("✅ TAY ĐIỀU KHIỂN ĐÃ SẴN SÀNG (ESP-NOW)!");
}

void loop() {
  readMPU6050(gocX, gocY, tocDoXoayZ);
  String cmd = getCommand(gocX, gocY, tocDoXoayZ);
  
  // Gửi lệnh mỗi 20ms (50Hz)
  if (millis() - lastSendTime >= 20) {
    sendCommand(cmd);
    lastSendTime = millis();
  }
  
  if (cmd != lastCmd) {
    lastCmd = cmd;
    Serial.print("📤 Lệnh: ");
    Serial.println(cmd);
  }
  
  delay(10);
}