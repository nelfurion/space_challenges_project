#include "sensors/CO2.h"
#include "sensors/Gas.h"

#define GAS_PIN A4

CO2 co2_sensor = CO2();
Gas gas_sensor = Gas(GAS_PIN);

void setup() {
  Serial1.begin(9600); // for co2 sensor
  Serial.begin(115200); // for data print
  gas_sensor.setup();
  Serial.println("calibrating CO2 sensor...");
  co2_sensor.setup();
  Serial.println("CO2 sensor calibration finished...");
}

void loop() {
  int gas_sensor_value = analogRead(GAS_PIN);
  GasData gas_data = gas_sensor.read(gas_sensor_value);
  printGasSensorData(gas_data);
  byte* sentCommands = sendCO2Commands();
  CO2Data co2_data = co2_sensor.read(sentCommands);
  printCO2Data(co2_data);
}

void printCO2Data(data) {
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.print("  CO2: ");
  Serial.print(data.CO2PPM);
  Serial.println("");
}

void printGasSensorData(GasData data) {
  Serial.print("sensor_volt = ");
  Serial.println(data.voltage);
  Serial.print("RS_ratio = ");
  Serial.println(data.RS_gas);
  Serial.print("Rs/R0 = ");
  Serial.println(data.ratio);

  Serial.print("\n\n");
}

byte* sendCO2Commands() {
  byte data[9];
  int i = 0;

  //transmit command data
  for(i=0; i<sizeof(co2_sensor.cmd_get_sensor); i++)
  {
      Serial1.write(co2_sensor.cmd_get_sensor[i]);
  }
  delay(10);
  //begin reveiceing data
  if(Serial1.available())
  {
      while(Serial1.available())
      {
          for(int i=0;i<9; i++)
          {
              data[i] = Serial1.read();
          }
      }
  }

  return data;
}
