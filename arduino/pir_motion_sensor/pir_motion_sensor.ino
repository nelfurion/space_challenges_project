/*macro definitions of PIR motion sensor pin and LED pin*/
#define PIR_MOTION_SENSOR 20
#define LED 8


// The sensor's radius is around 10cm

void setup()
{
    pinMode(PIR_MOTION_SENSOR, INPUT);
    pinMode(LED,OUTPUT);
}

void loop()
{
    if(isPeopleDetected())//if it detects the moving people?
        digitalWrite(LED, HIGH);
    else
        digitalWrite(LED, LOW);
}


/***************************************************************/
/*Function: Detect whether anyone moves in it's detecting range*/
/*Return:-boolean, true is someone detected.*/
boolean isPeopleDetected()
{
    int sensorValue = digitalRead(PIR_MOTION_SENSOR);
    if(sensorValue == HIGH)//if the sensor value is HIGH?
    {
        return true;//yes,return true
    }
    else
    {
        return false;//no,return false
    }
}
