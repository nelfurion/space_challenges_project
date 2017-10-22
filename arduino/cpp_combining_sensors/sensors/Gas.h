struct GasData {
  float voltage;
  float RS_gas;
  float ratio;
};

class Gas {
private:
  int inputPin;
public:
  void setup();
  GasData read(int sensorValue);

  Gas(int inputPin);
  ~Gas();
};
