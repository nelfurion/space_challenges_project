class Loudness {
private:
  int inputPin;
public:
  void setup();
  void read();

  Loudness(int inputPin);
  ~Loudness();
};
