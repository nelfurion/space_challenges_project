class PinConstants {
public:
  /*
  The TPH sensor uses the I^2C interface
  The GPS uses the UART interface

  1* Grove – Button - X

  1* Grove – Light sensor - X

  1* Grove – PIR motion sensor - V

  1* Grove – Temperature – pressure – humidity sensor - V

  1* Grove – 3-Axis Digital Accelerometer - V

  1* Grove – Sound/Loudness Sensor - V

  1* Grove – GPS module - V

  1* Grove – Air quality sensor - V

  1* Grove – Tilt switch - V

  1* Grove – Magnetic door switch & screw connector - X

  1* Grove – CO2 Sensor - X
  1* Particle sensors – Optical Dust Sensor - GP2Y1010AU0F - X


  */

  static struct Accelerometer {
    int input = A2;
  };

  static struct AirQuality {
    int input = A0;
  };

  static struct Loudness {
    int input = A4;
  };

  static struct PirMotion {
    int input = 20;
  };

  static struct TiltSwitch {
    int input = 4;
  }
}
