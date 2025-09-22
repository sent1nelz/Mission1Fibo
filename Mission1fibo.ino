#include <POP32.h>

#define CH1 PB10   // สไลด์ ซ้าย-ขวา (X)
#define CH2 PB3   // เดินหน้า-ถอยหลัง (Y)
#define CH4 PC14   // หมุนตัว L R
#define CH5 PA15   // สวิสต์ปั่น
#define CH6 PB11   // Speed knob
#define M1 2
#define M2 4
#define M3 1
#define M4 3

int ch1Value; 
int ch2Value;
int ch4Value;
int ch6Value;

// int mapSpeed(int speed) {
//   return map(constrain(speed, 0, 100), 0, 100, 0, 255);
// }

void moveForward(int speed) { // เดินหน้า 
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  Serial.print(speed);
  motor(M1, speed);
  motor(M2, speed);
  motor(M3, speed);
  motor(M4, speed);
}
void moveBackward(int speed) { // ถอยหลัง
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(M1, -speed);
  motor(M2, -speed);
  motor(M3, -speed);
  motor(M4, -speed);
}

void moveLeft(int speed) { // สไลด์ซ้าย
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(M1, -speed);
  motor(M2, speed);
  motor(M3, speed);
  motor(M4, -speed);
}

void moveRight(int speed) { // สไลด์ขวา
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(M1, speed);
  motor(M2, -speed);
  motor(M3, -speed);
  motor(M4, speed);
}

void moveClockw(int speed) { // หมุนตามเข็ม (ขวา)
  speed = abs(constrain(speed, 20, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(M1, speed);
  motor(M2, -speed);
  motor(M3, speed);
  motor(M4, -speed);
}

void moveCClockw(int speed) { // หมุนทวนเข็ม (ซ้าย)
  speed = abs(constrain(speed, 0, 100));
  speed = map(speed, 20, 100, 0, 100);
  motor(M1, -speed);
  motor(M2, speed);
  motor(M3, -speed);
  motor(M4, speed);
}

void stopmotors() {
  motor(M1, 0);
  motor(M2, 0);
  motor(M3, 0);
  motor(M4, 0);
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
}

void loop() {
  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch6Value = readChannel(CH6, 0, 100, 0);

  // แปลงค่าที่ได้เป็นทิศทาง
  String direction = "Stop";

  if (ch2Value > 20){direction = "Forward"; moveForward(ch2Value);}
  else if (ch2Value < -20){direction = "Backward"; moveBackward(abs(ch2Value));}

  else if (ch1Value > 20){direction = "Strafe Right"; moveRight(ch1Value);}
  else if (ch1Value < -20){direction = "Strafe Left"; moveLeft(abs(ch1Value));}

  else if (ch4Value > 20){direction = "Rotate Right"; moveClockw(ch4Value);}
  else if (ch4Value < -20){direction = "Rotate Left"; moveCClockw(abs(ch4Value));}

  else {direction = "Stop"; stopmotors();}

  Serial.print("X: "); Serial.print(ch1Value);
  Serial.print(" | Y: "); Serial.print(ch2Value);
  Serial.print(" | R: "); Serial.print(ch4Value);
  Serial.print(" --> ");
  Serial.println(direction);
  // Serial.print(" | Speed: "); Serial.println(ch6Value);

  delay(100);
}
