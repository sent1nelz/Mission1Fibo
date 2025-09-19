#define CH1 3
#define CH2 5
#define CH6 6   // หมุนรอบตัว (ใช้ knob)

#define FL1 7   // Front Left
#define FL2 8
#define FR1 9   // Front Right
#define FR2 10
#define RL1 11  // Rear Left
#define RL2 12
#define RR1 13  // Rear Right
#define RR2 4

int ch1Value;
int ch2Value;
int ch6Value; // R (จาก knob)

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setMotor(int pin1, int pin2, int speed) {
  speed = constrain(speed, -255, 255);
  if (speed > 0) {
    analogWrite(pin1, speed);
    analogWrite(pin2, 0);
  } else {
    analogWrite(pin1, 0);
    analogWrite(pin2, -speed);
  }
}

void mecanumDrive(int x, int y, int r) {
  int speedFL = y + x + r;
  int speedFR = y - x - r;
  int speedRL = y - x + r;
  int speedRR = y + x - r;

  setMotor(FL1, FL2, speedFL);
  setMotor(FR1, FR2, speedFR);
  setMotor(RL1, RL2, speedRL);
  setMotor(RR1, RR2, speedRR);
}

void setup() {
  Serial.begin(115200);

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH6, INPUT);

  pinMode(FL1, OUTPUT); pinMode(FL2, OUTPUT);
  pinMode(FR1, OUTPUT); pinMode(FR2, OUTPUT);
  pinMode(RL1, OUTPUT); pinMode(RL2, OUTPUT);
  pinMode(RR1, OUTPUT); pinMode(RR2, OUTPUT);
}

void loop() {
  ch1Value = readChannel(CH1, -255, 255, 0);
  ch2Value = readChannel(CH2, -255, 255, 0);
  ch6Value = readChannel(CH6, -255, 255, 0);

  Serial.print("X: "); Serial.print(ch1Value);
  Serial.print(" | Y: "); Serial.print(ch2Value);
  Serial.print(" | R: "); Serial.println(ch6Value);

  mecanumDrive(ch1Value, ch2Value, ch6Value);

  delay(50);
}
