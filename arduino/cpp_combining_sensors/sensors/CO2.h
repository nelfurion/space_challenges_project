struct CO2Data {
  unsigned char data[9];
  int temperature;
  int CO2PPM;
};

class CO2 {
public:
  const unsigned char cmd_calibrate[] =
  {
      0xff, 0x87, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf2
  };

  const unsigned char cmd_get_sensor[] =
  {
      0xff, 0x01, 0x86, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x79
  };

  void calibrate();
  void setup();
  void read(byte* data);

  CO2();
  ~CO2();
};
