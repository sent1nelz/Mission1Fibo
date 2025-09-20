#include <POP32.h>

#define CH1 3   // สไลด์ ซ้าย-ขวา (X)
#define CH2 5   // เดินหน้า-ถอยหลัง (Y)
#define CH4 4   // หมุนตัว L R
#define CH6 6   // Speed knob

int ch1Value; 
int ch2Value;
int ch4Value;
int ch6Value;

// int mapSpeed(int speed) {
//   return map(constrain(speed, 0, 100), 0, 100, 0, 255);
// }

void moveForward(int speed) { // เดินหน้า 
  speed = constrain(speed, 0, 100);
  motor(1, speed);
  motor(2, speed);
  motor(3, speed);
  motor(4, speed);
}
void moveBackward(int speed) { // ถอยหลัง
  speed = constrain(speed, 0, 100);
  motor(1, -speed);
  motor(2, -speed);
  motor(3, -speed);
  motor(4, -speed);
}

void moveLeft(int speed) { // สไลด์ซ้าย
  speed = constrain(speed, 0, 100);
  motor(1, -speed);
  motor(2, speed);
  motor(3, speed);
  motor(4, -speed);
}

void moveRight(int speed) { // สไลด์ขวา
  speed = constrain(speed, 0, 100);
  motor(1, speed);
  motor(2, -speed);
  motor(3, -speed);
  motor(4, speed);
}

void moveClockw(int speed) { // หมุนตามเข็ม (ขวา)
  speed = constrain(speed, 0, 100);
  motor(1, speed);
  motor(2, -speed);
  motor(3, speed);
  motor(4, -speed);
}

void moveCClockw(int speed) { // หมุนทวนเข็ม (ซ้าย)
  speed = constrain(speed, 0, 100);
  motor(1, -speed);
  motor(2, speed);
  motor(3, -speed);
  motor(4, speed);
}

void stopmotors(int speed) {
  speed = constrain(speed, 0, 100);
  motor(1, 0);
  motor(2, 0);
  motor(3, 0);
  motor(4, 0);
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
