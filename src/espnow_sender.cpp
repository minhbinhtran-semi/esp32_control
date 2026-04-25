#include "espnow_sender.h"

// MAC của xe (ESP32 30 chân)
uint8_t receiverMac[] = {0xB0, 0xCB, 0xD8, 0x03, 0xCD, 0x48};

static DataSend myData;
static bool peerAdded = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Không in ra Serial để tránh làm chậm
  // Có thể bỏ comment nếu cần debug
  // if (status != ESP_NOW_SEND_SUCCESS) {
  //   Serial.println("⚠️ Gửi thất bại");
  // }
}

void initESPNowSender() {
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb((esp_now_send_cb_t)OnDataSent);
  
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Không thể kết nối với Xe (kiểm tra MAC)");
    return;
  }
  
  peerAdded = true;
  Serial.println("✅ ESP-NOW Sender sẵn sàng!");
  Serial.print("📡 MAC xe: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", receiverMac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void sendCommand(String cmd) {
  if (!peerAdded) return;
  
  cmd.toCharArray(myData.cmd, sizeof(myData.cmd));
  esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));
}