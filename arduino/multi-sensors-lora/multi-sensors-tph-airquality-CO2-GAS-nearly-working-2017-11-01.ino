/*    _   _ _ _____ _    _              _____     _ _     ___ ___  _  __
 *   /_\ | | |_   _| |_ (_)_ _  __ _ __|_   _|_ _| | |__ / __|   \| |/ /
 *  / _ \| | | | | | ' \| | ' \/ _` (_-< | |/ _` | | / / \__ \ |) | ' <
 * /_/ \_\_|_| |_| |_||_|_|_||_\__, /__/ |_|\__,_|_|_\_\ |___/___/|_|\_\
 *                             |___/
 *
 * Copyright 2017 AllThingsTalk
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 *  AllThingsTalk Maker IoT experiment for LoRa
 *  Version 1.0 dd 09/11/2015
 *  Original author: Jan Bogaerts 2015
 *
 *  This sketch is part of the AllThingsTalk LoRa rapid development kit
 *  -> http://www.allthingstalk.com/lora-rapid-development-kit
 *
 *  This example sketch is based on the Proxilmus IoT network in Belgium
 *  The sketch and libs included support the
 *  - MicroChip RN2483 LoRa module
 *  - Embit LoRa modem EMB-LR1272
 *  
 *  External libraries used
 *  - Adafruit_BME280
 *  - Adafruit_Sensor
 *  - AirQuality2
 *  
 *  For more information, please check our documentation
 *  -> http://allthingstalk.com/docs/tutorials/lora/setup
 *
 * Explanation:
 * 
 * We will measure our environment using 6 sensors. Approximately, every
 * 5 minutes, all values will be read and sent to the AllthingsTalk Devloper
 * Cloud. 
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "AirQuality2.h"
#include <ATT_IOT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>
#include <PayloadBuilder.h>
#include <math.h>

#include "AirQuality.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

// This to be modified in order to integrate the CO2 sensor?
#define SERIAL_BAUD 57600

#define debugSerial Serial
#define loraSerial Serial1

#define AirQualityPin A0
//#define LightSensorPin A2
//#define SoundSensorPin A4

#define GAS_INPUT A2
#define SEALEVELPRESSURE_HPA (1013.25)
#define AIR_QUALITY_PIN A0

#define SEND_EVERY 5000

bool flag_calibration = true;

MicrochipLoRaModem modem(&loraSerial, &debugSerial);
ATTDevice device(&modem, &debugSerial, false, 3000);  // minimum time between 2 messages set at 5000 milliseconds

PayloadBuilder payload(device);

AirQuality2 airqualitysensor;
Adafruit_BME280 tph; // I2C

SoftwareSerial s_serial(A4, A5);      // TX, RX
//SoftwareSerial s_serial(20, 21);      // TX, RX
SoftwareSerial gps(20, 21);

#define CO2_SENSOR s_serial
unsigned char gps_buffer[64];
int gps_count=0;

struct all_variables {
  int current_quality =-1;
  
//  float soundValue;
//  float lightValue;
  float temperature;
  //float hum;
  //float pres;
  int16_t airValue;
  
//  unsigned char dataRevice[9]; // UNUSED
  int CO2PPM;

//air sensor
  float sensor_volt;
  float RS_gas; //  Get the value of RS via in a clear air
  float R0 = 1.15;  // Get the value of R0 via in LPG
  float sensorValue;
  float ratio;
  float ch4ppm;
  float coppm;
  float lpgppm;
};

all_variables data_collection;

/*
 *  CONNECTED SENSORS:
 *  AIRQUALITY - A0
 *  TPH - I^2C
 *  CO2 - //Serial1
 *  CO2 - RX: A4, TX: A5
 *  GAS Sensor - A2
 */


float soundValue;
float lightValue;
//float temp;
float hum;
float pres;
//int16_t airValue;

const unsigned char cmd_calibrate[] = 
{
    0xff, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2
};

void calibrate() {
  Serial.println("begin to calibrate");

  for(int i=0; i<sizeof(cmd_calibrate); i++)
  {
      Serial1.write(cmd_calibrate[i]);
  }

  Serial.println("calibrate done");
  flag_calibration=false;
}

const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};

bool dataRecieve(void)
{
    byte data[9];
    int i = 0;

    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        CO2_SENSOR.write(cmd_get_sensor[i]);
    }
    delay(10);
    //begin reveiceing data
    if(CO2_SENSOR.available())
    {
        while(CO2_SENSOR.available())
        {
            for(int i=0;i<9; i++)
            {
                data[i] = CO2_SENSOR.read();
            }
        }
    }
    for(int j=0; j<9; j++)
    {
        Serial.print(data[j]);
        Serial.print(" ");
    }
    Serial.println("");
/*
    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }*/

    data_collection.CO2PPM = (int)data[2] * 256 + (int)data[3];
    //data_collection.temperature = (int)data[4] - 40;

    return true;
}

void setup() 
{
  // CO2. If here works - remove from the loop body.
  CO2_SENSOR.begin(9600);
  Serial1.begin(9600);
  Serial.begin(115200);
  calibrate();
  Serial.println("get a 'g', begin to read from sensor!");
  Serial.println("********************************************************");
  Serial.println();

  
  pinMode(GROVEPWR, OUTPUT);  // turn on the power for the secondary row of grove connectors
  digitalWrite(GROVEPWR, HIGH);

  //calibrate();

  //airqualitysensor.init(AIR_QUALITY_PIN);

  debugSerial.begin(SERIAL_BAUD);
  debugSerial.begin(SERIAL_BAUD);
  while((!debugSerial) && (millis()) < 10000){}  // wait until the serial bus is available

  loraSerial.begin(modem.getDefaultBaudRate());  // set baud rate of the serial connection to match the modem
  while((!loraSerial) && (millis()) < 10000){}   // wait until the serial bus is available

  while(!device.initABP(DEV_ADDR, APPSKEY, NWKSKEY))
  debugSerial.println("Ready to send data");
  
  debugSerial.println();
  debugSerial.println("-- Environmental Sensing LoRa experiment --");
  debugSerial.println();

  initSensors();
}

void loop() 
{
 
  readSensors();
  displaySensorValues();
  if(dataRecieve())
  {
    Serial.print("Temperature: ");
    Serial.print(data_collection.temperature);
    Serial.print("  CO2: ");
    Serial.print(data_collection.CO2PPM);
    Serial.println("");
  }
  delay(1000);

  data_collection.sensorValue = analogRead(A2);
  data_collection.sensor_volt=data_collection.sensorValue/1024*5.0;
  data_collection.RS_gas = (5.0-data_collection.sensor_volt)/data_collection.sensor_volt; // omit *RL
  data_collection.ratio = data_collection.RS_gas/data_collection.R0;  // ratio = RS/R0
  data_collection.ch4ppm = pow ( 19.821/(data_collection.RS_gas/data_collection.R0), 2.799);
  data_collection.coppm = pow ( 14.882/(data_collection.RS_gas/data_collection.R0), 2.315);
  data_collection.lpgppm = pow ( 21.161/(data_collection.RS_gas/data_collection.R0), 2.222);
  
  sendSensorValues();
  
  debugSerial.print("Delay for: ");
  debugSerial.println(SEND_EVERY);
  debugSerial.println();
  delay(SEND_EVERY);
}

void initSensors()
{
  debugSerial.println("Initializing sensors, this can take a few seconds...");
  
//  pinMode(SoundSensorPin, INPUT);
//  pinMode(LightSensorPin, INPUT);
  
  tph.begin();
  airqualitysensor.init(AirQualityPin);
  debugSerial.println("Done");
}

void readSensors()
{
    debugSerial.println("Start reading sensors");
    debugSerial.println("---------------------");
    
//    soundValue = analogRead(SoundSensorPin);
//    lightValue = analogRead(LightSensorPin);
//    lightValue = lightValue * 3.3 / 1023;  // convert to lux based on the voltage that the sensor receives
//    lightValue = pow(10, lightValue);
    
    data_collection.temperature = tph.readTemperature();
    hum = tph.readHumidity();
    pres = tph.readPressure()/100.0;
    
    data_collection.airValue = airqualitysensor.getRawData();
}

void process()
{
  while(device.processQueue() > 0)
  {
    debugSerial.print("QueueCount: ");
    debugSerial.println(device.queueCount());
    delay(10000);
  }
}

void sendSensorValues()
{
  payload.reset();
  //payload.addInteger(1);  // identifier for this example
  //payload.addNumber(soundValue);
  //payload.addNumber(lightValue);
  Serial.print("Air value is: ");
  Serial.println(data_collection.airValue);
  payload.addInteger(data_collection.airValue);
  Serial.print("Temp is is: ");
  Serial.println(data_collection.temperature);
  payload.addNumber(data_collection.temperature);
  Serial.print("Hum is: ");
  Serial.println(hum);
  payload.addNumber(hum);
  Serial.print("Press is: ");
  Serial.println(pres);
  payload.addNumber(pres);
  Serial.print("CO2PPM is: ");
  Serial.println(data_collection.CO2PPM);
  payload.addInteger(data_collection.CO2PPM);
  Serial.print("Ratio of GAS is: ");
  Serial.println(data_collection.ratio);
  //payload.addNumber(data_collection.ratio);
  Serial.print("ch4pmm is: ");
  Serial.println(data_collection.ch4ppm);
  payload.addNumber(data_collection.ch4ppm);
  Serial.print("coppm is: ");
  Serial.println(data_collection.coppm);
  payload.addNumber(data_collection.coppm);
  Serial.print("lpgppm is: ");
  Serial.println(data_collection.lpgppm);
  payload.addNumber(data_collection.lpgppm);
  
  
  payload.addToQueue(false);
  process();
}

void displaySensorValues()
{    
  debugSerial.print("Temperature: ");
  debugSerial.print(data_collection.temperature);
  debugSerial.println(" °C");
      
  debugSerial.print("Humidity: ");
  debugSerial.print(hum);
  debugSerial.println(" %");
      
  debugSerial.print("Pressure: ");
  debugSerial.print(pres);
  debugSerial.println(" hPa");
  
  debugSerial.print("Air quality: ");
  debugSerial.print(data_collection.airValue);
  debugSerial.println(" Analog (0-1023)");
}

