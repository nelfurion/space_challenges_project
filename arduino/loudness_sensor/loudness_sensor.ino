#define LOUDNESS_SENSOR_PIN A4

int val;
void setup()
{
    Serial.begin(9600);
    pinMode(LOUDNESS_SENSOR_PIN, INPUT);
}

void loop()
{
    //analogRead(LOUDNESS_SENSOR_PIN);
    //delay(10);
    val = analogRead(LOUDNESS_SENSOR_PIN);
    Serial.println(val);
}
