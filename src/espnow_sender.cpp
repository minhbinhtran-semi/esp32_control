#include "espnow_sender.h"

// ❗ THAY MAC NÀY BẰNG MAC CỦA XE
// Cách tìm: Chạy code tìm MAC trên xe, in ra Serial
uint8_t receiverMac[] = {0x24, 0x6F, 0x28, 0x12, 0x34, 0x56};  // ← SỬA LẠI!

static DataSend myData;
static bool peerAdded = false;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    static unsigned long lastError = 0;
    if (millis() - lastError > 2000) {
      Serial.println("⚠️ Gửi thất bại, xe có thể ngoài tầm sóng");
      lastError = millis();
    }
  }
}

void initESPNowSender() {
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW");
    return;
  }
  
  esp_now_register_send_cb(OnDataSent);
  
  // Thêm peer (xe)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Lỗi thêm peer (kiểm tra MAC)");
    return;
  }
  
  peerAdded = true;
  Serial.println("✅ ESP-NOW Sender sẵn sàng!");
  Serial.print("📡 MAC đích: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", receiverMac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

void sendCommand(String cmd) {
  if (!peerAdded) return;
  
  cmd.toCharArray(myData.command, sizeof(myData.command));
  esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));
}