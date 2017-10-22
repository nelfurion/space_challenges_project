/*
  This test code is write for Arduino AVR Series(UNO, Leonardo, Mega)
  If you want to use with LinkIt ONE, please connect the module to D0/1 and modify:

  // #include <SoftwareSerial.h>
  // SoftwareSerial s_serial(2, 3);      // TX, RX

  #define sensor Serial1
*/

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

unsigned char dataRevice[9];
int temperature;
int CO2PPM;

void setup()
{
    Serial1.begin(9600);
    Serial.begin(115200);
    
    calibrate();
    
    Serial.println("get a 'g', begin to read from sensor!");
    Serial.println("********************************************************");
    Serial.println();
}

void loop()
{
    if(dataRecieve())
    {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("  CO2: ");
        Serial.print(CO2PPM);
        Serial.println("");
    }
    delay(1000);
}

bool dataRecieve(void)
{
    byte data[9];
    int i = 0;

    //transmit command data
    for(i=0; i<sizeof(cmd_get_sensor); i++)
    {
        Serial1.write(cmd_get_sensor[i]);
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

    for(int j=0; j<9; j++)
    {
        Serial.print(data[j]);
        Serial.print(" ");
    }
    Serial.println("");

    if((i != 9) || (1 + (0xFF ^ (byte)(data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7]))) != data[8])
    {
        return false;
    }

    CO2PPM = (int)data[2] * 256 + (int)data[3];
    temperature = (int)data[4] - 40;

    return true;
}
