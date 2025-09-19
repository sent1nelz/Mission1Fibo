#define CH1 3   // สไลด์ ซ้าย-ขวา (X)
#define CH2 5   // เดินหน้า-ถอยหลัง (Y)
#define CH4 4   // หมุนตัว L R
#define CH6 6   // Speed knob

#define M1_IN1 1    //หน้า-ซ้าย (Front Left)
#define M1_IN2 2    //หน้า-ซ้าย (Front Left)
#define M2_IN1 7    //หน้า-ขวา (Front Right)
#define M2_IN2 8    //หน้า-ขวา (Front Right)
#define M3_IN1 9    //หลัง-ซ้าย (Rear Left)
#define M3_IN2 10    //หลัง-ซ้าย (Rear Left)
#define M4_IN1 11    //หลัง-ขวา (Rear Right)
#define M4_IN2 12    //หลัง-ขวา (Rear Right)

int ch1Value; 
int ch2Value;
int ch4Value;
int ch6Value;

int mapSpeed(int speed) {
  return map(constrain(speed, 0, 100), 0, 100, 0, 255);
}

void setMotor(int in1, int in2, int speed) {
  int pwm = mapSpeed(abs(speed));
  if (speed > 0) {
    analogWrite(in1, pwm);
    analogWrite(in2, 0);
  } else if (speed < 0) {
    analogWrite(in1, 0);
    analogWrite(in2, pwm);
  } else {
    analogWrite(in1, 0);
    analogWrite(in2, 0);
  }
}

void moveForward(int speed) { // เดินหน้า
  setMotor(M1_IN1, M1_IN2, speed);
  setMotor(M2_IN1, M2_IN2, speed);
  setMotor(M3_IN1, M3_IN2, speed);
  setMotor(M4_IN1, M4_IN2, speed);
}

void moveBackward(int speed) { // ถอยหลัง
  setMotor(M1_IN1, M1_IN2, -speed);
  setMotor(M2_IN1, M2_IN2, -speed);
  setMotor(M3_IN1, M3_IN2, -speed);
  setMotor(M4_IN1, M4_IN2, -speed);
}

void moveLeft(int speed) { // สไลด์ซ้าย
  setMotor(M1_IN1, M1_IN2, -speed);
  setMotor(M2_IN1, M2_IN2, speed);
  setMotor(M3_IN1, M3_IN2, speed);
  setMotor(M4_IN1, M4_IN2, -speed);
}

void moveRight(int speed) { // สไลด์ขวา
  setMotor(M1_IN1, M1_IN2, speed);
  setMotor(M2_IN1, M2_IN2, -speed);
  setMotor(M3_IN1, M3_IN2, -speed);
  setMotor(M4_IN1, M4_IN2, speed);
}

void moveClockw(int speed) { // หมุนตามเข็ม (ขวา)
  setMotor(M1_IN1, M1_IN2, speed);
  setMotor(M2_IN1, M2_IN2, -speed);
  setMotor(M3_IN1, M3_IN2, speed);
  setMotor(M4_IN1, M4_IN2, -speed);
}

void moveCClockw(int speed) { // หมุนทวนเข็ม (ซ้าย)
  setMotor(M1_IN1, M1_IN2, -speed);
  setMotor(M2_IN1, M2_IN2, speed);
  setMotor(M3_IN1, M3_IN2, -speed);
  setMotor(M4_IN1, M4_IN2, speed);
}

void stopMotors() {
  setMotor(M1_IN1, M1_IN2, 0);
  setMotor(M2_IN1, M2_IN2, 0);
  setMotor(M3_IN1, M3_IN2, 0);
  setMotor(M4_IN1, M4_IN2, 0);
}

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

void setup() {
  Serial.begin(115200);

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH6, INPUT);

  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);
}

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch6Value = readChannel(CH6, 0, 100, 0);

  // แปลงค่าที่ได้เป็นทิศทาง
  String direction = "Stop";

  if (abs(ch1Value) < 15 && abs(ch2Value) < 15 && abs(ch4Value) < 15 && abs(ch6Value) <= 20){
    direction = "Stop";
  } else {
    if (ch2Value > 20 && abs(ch6Value) >= 20){direction = "Forward"; moveForward(ch6Value);}
    else if (ch2Value < -20 && abs(ch6Value) >= 20){direction = "Backward"; moveBackward(ch6Value);}

    if (ch1Value > 20 && abs(ch6Value) >= 20){direction = "Strafe Right"; moveRight(ch6Value);}
    else if (ch1Value < -20 && abs(ch6Value) >= 20){direction = "Strafe Left"; moveLeft(ch6Value);}

    if (ch4Value > 20 && abs(ch6Value) >= 20){direction = "Rotate Right"; moveClockw(ch6Value);}
    else if (ch4Value < -20 && abs(ch6Value) >= 20){direction = "Rotate Left"; moveCClockw(ch6Value);}
  }

  Serial.print("X: "); Serial.print(ch1Value);
  Serial.print(" | Y: "); Serial.print(ch2Value);
  Serial.print(" | R: "); Serial.print(ch4Value);
  Serial.print(" --> ");
  Serial.print(direction);
  Serial.print(" | Speed: "); Serial.println(ch6Value);

  delay(100);
}
