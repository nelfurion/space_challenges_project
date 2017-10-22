#include "Accelerometer.h"
#include "MMA7660.h"

MMA7660 accelemeter;

void Accelerometer::setup() {
  accelemeter.init();
}

void Accelerometer::readAcceleration(int8_t* x, int8_t* y, int8_t* z) {
  accelemeter.getAcceleration(&ax,&ay,&az);
}

void Accelerometer::readXYZ(int8_t* x, int8_t* y, int8_t* z) {
  accelemeter.getXYZ(&x,&y,&z);
}
