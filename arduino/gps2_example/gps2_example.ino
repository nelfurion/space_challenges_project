//#include <SoftwareSerial.h>
//SoftwareSerial Serial1(2, 3);
unsigned char buffer[64];                   // buffer array for data receive over serial port
int count=0;                                // counter for buffer array
void setup()
{
    Serial1.begin(9600);                 // the Serial1 baud rate
    Serial.begin(9600);                     // the Serial port of Arduino baud rate.
}

void loop()
{
    if (Serial1.available())                     // if date is coming from software serial port ==> data is coming from Serial1 shield
    {
        while(Serial1.available())               // reading data into char array
        {
            buffer[count++]=Serial1.read();      // writing data into array
            if(count == 64)break;
        }
        Serial.write(buffer,count);                 // if no data transmission ends, write buffer to hardware serial port
        clearBufferArray();                         // call clearBufferArray function to clear the stored data from the array
        count = 0;                                  // set counter of while loop to zero 
    }
    if (Serial.available())                 // if data is available on hardware serial port ==> data is coming from PC or notebook
    Serial1.write(Serial.read());        // write it to the Serial1 shield
}


void clearBufferArray()                     // function to clear buffer array
{
    for (int i=0; i<count;i++)
    {
        buffer[i]=NULL;
    }                      // clear all index of array with command NULL
}
