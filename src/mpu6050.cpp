#include "mpu6050.h"

void initMPU6050() {
  Wire.begin(MPU_SDA, MPU_SCL);
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1B);
  Wire.write(0x08); // Gyro scale: 500 deg/s
  Wire.endTransmission(true);
}

void readMPU6050(float &gocX, float &gocY, float &tocDoXoayZ) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);

  if (Wire.available() >= 14) {
    int16_t ax = Wire.read() << 8 | Wire.read();
    int16_t ay = Wire.read() << 8 | Wire.read();
    int16_t az = Wire.read() << 8 | Wire.read();
    
    Wire.read(); Wire.read();
    
    int16_t gx = Wire.read() << 8 | Wire.read();
    int16_t gy = Wire.read() << 8 | Wire.read();
    int16_t gz = Wire.read() << 8 | Wire.read();

    gocX = -(atan2(ay / 16384.0, az / 16384.0) * 180.0 / PI);
    gocY = atan2(-ax / 16384.0, sqrt((ay / 16384.0) * (ay / 16384.0) + (az / 16384.0) * (az / 16384.0))) * 180.0 / PI;
    tocDoXoayZ = gz / 65.5;
  }
}

String getCommand(float tiltX, float tiltY, float spinZ) {
  const float NGUONG_TILT = 20.0;
  const float NGUONG_SPIN = 60.0;

  if (spinZ > NGUONG_SPIN) return "X_TRAI";
  if (spinZ < -NGUONG_SPIN) return "X_PHAI";
  if (tiltX > NGUONG_TILT) return "TRAI";
  if (tiltX < -NGUONG_TILT) return "PHAI";
  if (tiltY < -NGUONG_TILT) return "TIEN";
  if (tiltY > NGUONG_TILT) return "LUI";
  return "DUNG";
}