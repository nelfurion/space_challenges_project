#include <Adafruit_BME280.h>
#include "AirQuality.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include "AirQuality2.h"
#include <ATT_IOT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>
#include <PayloadBuilder.h>

#define SERIAL_BAUD 57600

#define debugSerial Serial
#define loraSerial Serial1

#define GAS_INPUT A4
#define SEALEVELPRESSURE_HPA (1013.25)
#define AIR_QUALITY_PIN A0

#define SEND_EVERY 300000

MicrochipLoRaModem modem(&loraSerial, &debugSerial);
ATTDevice device(&modem, &debugSerial, false, 10000);  // minimum time between 2 messages set at 7000 milliseconds

PayloadBuilder payload(device);

AirQuality airqualitysensor;

Adafruit_BME280 bme; // I2C

SoftwareSerial s_serial(4, 5);      // TX, RX
SoftwareSerial gps(20, 21);

#define CO2_SENSOR s_serial
unsigned char gps_buffer[64];
int gps_count=0;

struct all_variables {
  int current_quality =-1;
  
//  float soundValue;
//  float lightValue;
  int temperature;
  float hum;
  float pres;
  int16_t airValue;
  
//  unsigned char dataRevice[9];
  int CO2PPM;

//air sensor
  float sensor_volt;
  float RS_air; //  Get the value of RS via in a clear air
  float R0;  // Get the value of R0 via in LPG
  float sensorValue;
};

all_variables data_collection;

/*
 *  CONNECTED SENSORS:
 *  AIRQUALITY - A0
 *  TPH - I^2C
 *  CO2 - //Serial1
 *  CO2 - RX: D4, TX: D5
 *  GAS Sensor - A4
 */



/*void setup() {
    Serial.begin(9600);
}

float R0 = 1.15;

void loop() {

    float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    int sensorValue = analogRead(A4);
    sensor_volt=(float)sensorValue/1024*5.0;
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL

          /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
/*    ratio = RS_gas/R0;  // ratio = RS/R0
          /*-----------------------------------------------------------------------*/

/*    Serial.print("sensor_volt = ");
    Serial.println(sensor_volt);
    Serial.print("RS_ratio = ");
    Serial.println(RS_gas);
    Serial.print("Rs/R0 = ");
    Serial.println(ratio);

    Serial.print("\n\n");

    delay(1000);

}
*/

/* CO2 calibration */

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
}

const unsigned char cmd_get_sensor[] =
{
    0xff, 0x01, 0x86, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x79
};

/* Gas Calibration */

void setup() {
    Serial.begin(9600);
    CO2_SENSOR.begin(9600);
    gps.begin(9600);

    if (!bme.begin()) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
    }
    
    calibrate();

    airqualitysensor.init(AIR_QUALITY_PIN);
    
    Serial.println("get a 'g', begin to read from sensor!");
    Serial.println("********************************************************");
    Serial.println();
}

void loop() {
    /*--- Get a average data by testing 100 times ---*/
    
    for(int x = 0 ; x < 100 ; x++)
    {
        data_collection.sensorValue = data_collection.sensorValue + analogRead(GAS_INPUT);
    }
    data_collection.sensorValue = data_collection.sensorValue/100.0;
    /*-----------------------------------------------*/

    data_collection.sensor_volt = data_collection.sensorValue/1024*5.0;
    data_collection.RS_air = (5.0-data_collection.sensor_volt)/data_collection.sensor_volt; // omit *RL
    data_collection.R0 = data_collection.RS_air/9.9; // The ratio of RS/R0 is 9.9 in LPG gas from Graph (Found using WebPlotDigitizer)

    //Serial.print("sensor_volt = ");
    //Serial.print(data_collection.sensor_volt);
    //Serial.println("V");

    //Serial.print("R0 = ");
    //Serial.println(data_collection.R0);

  //  Serial.println("\n\n");

    //if(dataRecieve())
    //{
    //    Serial.print("Temperature: ");
   //     Serial.print(data_collection.temperature);
   //     Serial.print("  CO2: ");
   //     Serial.print(data_collection.CO2PPM);
    //    Serial.println("");
   // }

    Serial.println("\n");
 //   printTPHData();
    readTPHData();

    Serial.println("\n");
    printAirQualityData();

    Serial.println("\n");
    printGPSData();

  sendSensorValues();
  
  debugSerial.print("Delay for: ");
  debugSerial.println(SEND_EVERY);
  debugSerial.println();
  delay(SEND_EVERY);

//   Serial.println("----------------------------------\n");
//    delay(1000);
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
  payload.addNumber(data_collection.temperature);
/*  payload.addNumber(data_collection.hum);
  payload.addNumber(data_collection.pres);
  payload.addNumber(data_collection.sensor_volt);
  payload.addNumber(data_collection.RS_air);
  payload.addNumber(data_collection.R0);
  payload.addNumber(data_collection.sensorValue);
  payload.addInteger(data_collection.CO2PPM);
  payload.addInteger(data_collection.airValue);
  Serial.print(data_collection.temperature);
  Serial.print(data_collection.hum);
  Serial.print(data_collection.pres);
  Serial.print(data_collection.sensor_volt);
  Serial.print(data_collection.RS_air);
  Serial.print(data_collection.R0);
  Serial.print(data_collection.sensorValue);
  Serial.print(data_collection.CO2PPM);
  Serial.print(data_collection.airValue);*/
  payload.addToQueue(false);
  process();
}

void readTPHData(){
  data_collection.temperature = bme.readTemperature();
  data_collection.hum = bme.readHumidity();
  data_collection.pres = bme.readPressure() / 100.0F;
}

void printTPHData() {
    Serial.println("TPH: \n");
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
}

void printAirQualityData() {
    data_collection.current_quality=airqualitysensor.slope();
    /*if (data_collection.current_quality >= 0)// if a valid data returned.
    {
        if (data_collection.current_quality==0)
            //Serial.println("High pollution! Force signal active");
        else if (data_collection.current_quality==1)
            //Serial.println("High pollution!");
        else if (data_collection.current_quality==2)
            //Serial.println("Low pollution!");
        else if (data_collection.current_quality ==3)
            //Serial.println("Fresh air");
    }*/
}

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

   // for(int j=0; j<9; j++)
   // {
   //     Serial.print(data[j]);
    //    Serial.print(" ");
   // }
   // Serial.println("");

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    data_collection.CO2PPM = (int)data[2] * 256 + (int)data[3];
    data_collection.temperature = (int)data[4] - 40;

    return true;
}

ISR(TIMER2_OVF_vect)
{
    //default 122 - 2 seconds
    if(airqualitysensor.counter==80)//set 2 seconds as a detected duty
    {
        airqualitysensor.last_vol=airqualitysensor.first_vol;
        airqualitysensor.first_vol=analogRead(AIR_QUALITY_PIN);
        airqualitysensor.counter=0;
        airqualitysensor.timer_index=1;
        PORTB=PORTB^0x20;
    }
    else
    {
        airqualitysensor.counter++;
    }
}

void printGPSData() {
  if (gps.available())                     // if date is coming from software serial port ==> data is coming from Serial1 shield
    {
       // Serial.println("GPS DATA:");
        while(gps.available())               // reading data into char array
        {
            gps_buffer[gps_count++]=gps.read();      // writing data into array
            if(gps_count == 64)break;
        }
       // Serial.write(gps_buffer,gps_count);                 // if no data transmission ends, write buffer to hardware serial port
        clearGPSBufferArray();                         // call clearBufferArray function to clear the stored data from the array
        gps_count = 0;                                  // set counter of while loop to zero 
    }
    if (Serial.available()) {
        gps.write(Serial.read());
    }
}

void clearGPSBufferArray()                     // function to clear buffer array
{
    for (int i=0; i<gps_count;i++)
    {
        gps_buffer[i]=NULL;
    }                      // clear all index of array with command NULL
}
