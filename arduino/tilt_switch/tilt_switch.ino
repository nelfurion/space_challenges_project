#define TILT_SWITCH_INPUT 4 // does not work on pin 10 for some reason !@#?!@?#?!@?#?
#define GREEN_LED 8
#define RED_LED 9

void setup()
{
    pinMode(TILT_SWITCH_INPUT , INPUT);

    //GREEN LED
    pinMode(GREEN_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);

    //RED LED 
    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, LOW);
}

void loop()
{
    if (digitalRead(TILT_SWITCH_INPUT)==HIGH) {
      digitalWrite(GREEN_LED, HIGH);
      delay(200);
      digitalWrite(RED_LED, LOW);
    }
    else {
      digitalWrite(GREEN_LED, LOW);
      delay(200);
      digitalWrite(RED_LED, HIGH);
    }
}
