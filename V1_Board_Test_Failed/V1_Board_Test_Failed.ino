#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

const uint8_t PCA_ADDRESS = 0x40;
const uint8_t SERVO_CHANNEL = 15; // Match the number printed above the plug.
const int CENTER_US = 1500;
const int LOW_US = 1300;
const int HIGH_US = 1700;
Adafruit_PWMServoDriver pwm(PCA_ADDRESS);
int currentUs = CENTER_US;

void moveSlowly(int targetUs) {
  while (currentUs != targetUs) {
    currentUs += (targetUs > currentUs) ? 1 : -1;
    pwm.writeMicroseconds(SERVO_CHANNEL, currentUs);
    delay(20);
  }
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(PCA_ADDRESS);
  if (Wire.endTransmission() != 0) {
    Serial.println(F("PCA9685 not found at 0x40. Check SDA/SCL/VCC/GND."));
    while (true) { delay(1000); }
  }
  if (!pwm.begin()) {
    Serial.println(F("PCA9685 initialization failed."));
    while (true) { delay(1000); }
  }
  pwm.setPWMFreq(50);
  delay(10);
  for (uint8_t ch = 0; ch < 16; ch++) pwm.setPWM(ch, 0, 4096);
  pwm.writeMicroseconds(SERVO_CHANNEL, CENTER_US);
  delay(2000);
  Serial.println(F("Ready: one servo, 1300-1700 us, 50 Hz."));
}

void loop() {
  Serial.println(F("Moving to LOW"));
  moveSlowly(LOW_US);
  Serial.println(F("Moving to HIGH"));
  moveSlowly(HIGH_US);
  Serial.println(F("Moving to CENTER"));
  moveSlowly(CENTER_US);
}
