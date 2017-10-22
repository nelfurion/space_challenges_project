#include "Gas.h"

void Gas::setup() {}

float R0 = 1.15;

GasData Gas::read(int sensorValue) {
  float sensor_volt;
  float RS_gas; // Get value of RS in a GAS
  float ratio; // Get ratio RS_GAS/RS_air
  sensor_volt=(float)sensorValue/1024*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL

        /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
  ratio = RS_gas/R0;  // ratio = RS/R0
        /*-----------------------------------------------------------------------*/

  GasData data;
  data.RS_gas = RS_gas;
  data.ratio = ratio;
  data.voltage = sensor_volt;

  return data;
}

Gas::Gas(int inputPin) {
  this->inputPin = inputPin;
}

Gas::~Gas() { }
