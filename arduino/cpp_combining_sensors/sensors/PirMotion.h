class PirMotion {
public:
  void setup();
  void readAcceleration(int8_t* x, int8_t* y, int8_t* z);
  void readXYZ(int8_t* x, int8_t* y, int8_t* z);

  Accelerometer(int inputPin);
  ~Accelerometer();
};
