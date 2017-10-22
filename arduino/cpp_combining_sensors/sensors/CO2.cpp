#include "CO2.h"

/*
  This test code is write for Arduino AVR Series(UNO, Leonardo, Mega)
  If you want to use with LinkIt ONE, please connect the module to D0/1 and modify:

  // #include <SoftwareSerial.h>
  // SoftwareSerial s_serial(2, 3);      // TX, RX

  #define sensor Serial1
*/

void CO2::calibrate() {
  for(int i=0; i<sizeof(cmd_calibrate); i++)
  {
      Serial1.write(cmd_calibrate[i]);
  }
}

CO2Data data;

void CO2::setup() {
  calibrate();
}

CO2Data CO2::read(byte* data) {
  dataRecieve(data);

  return data;
}

CO2::CO2();
CO2::~CO2();

bool dataRecieve(byte* data)
{
    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    data.CO2PPM = (int)data[2] * 256 + (int)data[3];
    data.temperature = (int)data[4] - 40;

    return true;
}
