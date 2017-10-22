class AirQuality {
private:
  AirQuality airqualitySensor;
public:
  void setup();
  int readCurrentQuality();

  AirQuality(int inputPin);
  ~AirQuality();
};
