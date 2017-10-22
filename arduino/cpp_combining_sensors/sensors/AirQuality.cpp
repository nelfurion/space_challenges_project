#include "AirQuality.h"
#include "Arduino.h"

AirQuality airqualitysensor;

void AirQuality::setup(int pin) {
  airqualitysensor.init(pin);
  AirQuality::airqualitySensor
}

void AirQuality::readSlope() {
  airqualitysensor.slope();
}

void AirQuality::readXYZ(int8_t* x, int8_t* y, int8_t* z) {
  accelemeter.getXYZ(&x,&y,&z);
}

ISR(TIMER2_OVF_vect)
{
    if(airqualitysensor.counter==122)//set 2 seconds as a detected duty
    {

        airqualitysensor.last_vol=airqualitysensor.first_vol;
        airqualitysensor.first_vol=analogRead(A0); // change this value if you use another A port
        airqualitysensor.counter=0;
        airqualitysensor.timer_index=1;
        PORTB=PORTB^0x20;
    }
    else
    {
        airqualitysensor.counter++;
    }
}
