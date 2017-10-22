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

#include <ATT_IOT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>
#include <PayloadBuilder.h>

#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

#define SERIAL_BAUD 57600

#define debugSerial Serial
#define loraSerial Serial1

MicrochipLoRaModem modem(&loraSerial, &debugSerial);
ATTDevice device(&modem, &debugSerial, false, 3000);  // minimum time between 2 messages set at 7000 milliseconds

PayloadBuilder payload(device);

int digitalSensor = 20;  // digital sensor is connected to pin D20/21

void setup() 
{  
  Serial.begin(9600);
  pinMode(digitalSensor, INPUT);  // initialize the digital pin as an input
  delay(3000);
  
  debugSerial.begin(SERIAL_BAUD);
  while((!debugSerial) && (millis()) < 10000){}  // wait until the serial bus is available
  
  loraSerial.begin(modem.getDefaultBaudRate());  // set baud rate of the serial connection to match the modem
  while((!loraSerial) && (millis()) < 10000){}   // wait until the serial bus is available

  while(!device.initABP(DEV_ADDR, APPSKEY, NWKSKEY))
  debugSerial.println("Ready to send data");

   if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
   }

  // send initial value
  sendValue(false, 0, 0, 0, 0);
}

void sendValue(bool val, float temp, float humid, float altit, float pres)
{
  payload.reset();
  for (int i = 0; i<15; i++){
    payload.addNumber(temp);
  }
  payload.addBoolean(val);
  payload.addNumber(temp);
  payload.addNumber(humid);
  payload.addNumber(altit);
  payload.addNumber(pres);
  payload.addToQueue(false);  // without ACK
  
  while(device.processQueue() > 0)
  {
    debugSerial.print("QueueCount: ");
    debugSerial.println(device.queueCount());
    delay(10000);
  }
}

bool sensorVal = false;
bool prevButtonState = false;

void loop() 
{
  for (int i = 0; i<15; i++){
    
  }
  //float fict1, fict2, fict3, fict4, fict5, fict6, fict7, fict8, fict9, fict10, fict11, fict12, fict13, fict14, fict15;
  float temp = bme.readTemperature();
  float pres = bme.readPressure() / 100.0F;
  float altit = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humid = bme.readHumidity();
  //Serial.print("Temperature = ");
  //Serial.print(temp);
  
  bool sensorRead = digitalRead(digitalSensor);     // read status Digital Sensor
  if (sensorRead == 1 && prevButtonState == false)  // verify if value has changed
  {
    prevButtonState = true;
    debugSerial.println("Button pressed");
    sendValue(!sensorVal, temp, humid, altit, pres);
    sensorVal = !sensorVal;
  }
  else if(sensorRead == 0)
    prevButtonState = false;
}
