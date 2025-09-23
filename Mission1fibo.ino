#define CH1 3   // สไลด์ ซ้าย-ขวา (X)
#define CH2 5   // เดินหน้า-ถอยหลัง (Y)
#define CH4 4   // หมุนตัว L R

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

void moveForward(int speed) { // เดินหน้า
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);

  analogWrite(M1_IN1, speed);
  analogWrite(M1_IN2, 0);

  analogWrite(M2_IN1, speed);
  analogWrite(M2_IN2, 0);

  analogWrite(M3_IN1, speed);
  analogWrite(M3_IN2, 0);

  analogWrite(M4_IN1, speed);
  analogWrite(M4_IN2, 0);
}

void moveBackward(int speed) { // ถอยหลัง
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);

  analogWrite(M1_IN1, 0);
  analogWrite(M1_IN2, speed);

  analogWrite(M2_IN1, 0);
  analogWrite(M2_IN2, speed);

  analogWrite(M3_IN1, 0);
  analogWrite(M3_IN2, speed);

  analogWrite(M4_IN1, 0);
  analogWrite(M4_IN2, speed);
}

void moveLeft(int speed) { // สไลด์ซ้าย
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  
  analogWrite(M1_IN1, 0);
  analogWrite(M1_IN2, speed);

  analogWrite(M2_IN1, speed);
  analogWrite(M2_IN2, 0);

  analogWrite(M3_IN1, speed);
  analogWrite(M3_IN2, 0);

  analogWrite(M4_IN1, 0);
  analogWrite(M4_IN2, speed);
}

void moveRight(int speed) { // สไลด์ขวา
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);

  analogWrite(M1_IN1, speed);
  analogWrite(M1_IN2, 0);

  analogWrite(M2_IN1, 0);
  analogWrite(M2_IN2, speed);

  analogWrite(M3_IN1, 0);
  analogWrite(M3_IN2, speed);

  analogWrite(M4_IN1, speed);
  analogWrite(M4_IN2, 0);
}

void moveClockw(int speed) { // หมุนตามเข็ม (ขวา)
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);

  analogWrite(M1_IN1, speed);
  analogWrite(M1_IN2, 0);

  analogWrite(M2_IN1, 0);
  analogWrite(M2_IN2, speed);

  analogWrite(M3_IN1, speed);
  analogWrite(M3_IN2, 0);

  analogWrite(M4_IN1, 0);
  analogWrite(M4_IN2, speed);
}

void moveCClockw(int speed) { // หมุนทวนเข็ม (ซ้าย)
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);

  analogWrite(M1_IN1, 0);
  analogWrite(M1_IN2, speed);

  analogWrite(M2_IN1, speed);
  analogWrite(M2_IN2, 0);

  analogWrite(M3_IN1, 0);
  analogWrite(M3_IN2, speed);

  analogWrite(M4_IN1, speed);
  analogWrite(M4_IN2, 0);
}

void stopMotors() {

  analogWrite(M1_IN1, 0);
  analogWrite(M1_IN2, 0);

  analogWrite(M2_IN1, 0);
  analogWrite(M2_IN2, 0);

  analogWrite(M3_IN1, 0);
  analogWrite(M3_IN2, 0);

  analogWrite(M4_IN1, 0);
  analogWrite(M4_IN2, 0);
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

  pinMode(M1_IN1, OUTPUT); pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT); pinMode(M2_IN2, OUTPUT);
  pinMode(M3_IN1, OUTPUT); pinMode(M3_IN2, OUTPUT);
  pinMode(M4_IN1, OUTPUT); pinMode(M4_IN2, OUTPUT);
}

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch4Value = readChannel(CH4, -100, 100, 0);

  // แปลงค่าที่ได้เป็นทิศทาง
  String direction = "Stop";

  if (ch2Value > 20){direction = "Forward"; moveForward(ch2Value);}
  else if (ch2Value < -20){direction = "Backward"; moveBackward(abs(ch2Value));}

  else if (ch1Value > 20){direction = "Strafe Right"; moveRight(ch1Value);}
  else if (ch1Value < -20){direction = "Strafe Left"; moveLeft(abs(ch1Value));}

  else if (ch4Value > 20){direction = "Rotate Right"; moveClockw(ch4Value);}
  else if (ch4Value < -20){direction = "Rotate Left"; moveCClockw(abs(ch4Value));}

  else {direction = "Stop"; stopMotors();}

  Serial.print("X: "); Serial.print(ch1Value);
  Serial.print(" | Y: "); Serial.print(ch2Value);
  Serial.print(" | R: "); Serial.print(ch4Value);
  Serial.print(" --> ");
  Serial.println(direction);
}
