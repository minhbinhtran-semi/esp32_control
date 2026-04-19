#ifndef MPU6050_H
#define MPU6050_H

#include <Arduino.h>
#include <Wire.h>

#define MPU_ADDR 0x68
#define MPU_SDA 4
#define MPU_SCL 5

void initMPU6050();
void readMPU6050(float &gocX, float &gocY, float &tocDoXoayZ);
String getCommand(float tiltX, float tiltY, float spinZ);

#endif