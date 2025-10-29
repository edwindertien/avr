/*
    TinyWalker using ATtiny85 @ 8MHz internal clock (ATtiny45 also possible)
 
 Everything is coded using the standard internal timer - millis() and a hard delay
 
 Startup sets a counter so servo's stay for 5 seconds in center position. Afther
 that the robot starts to walk. A dark sensor input (obstacle, hand) sets a 
 timer to reverse the walking pattern for 1 second.
             _________
 RESET      | 1     8 |  VCC
 PIN3 (A3)  | 2     7 |  PIN2 (A1,SCK,INT0)
 PIN4 (A2)  | 3     6 |  PIN1 (PWM, MISO)
 GND        |_4_____5_|  PIN0 (PWM,Aref,MOSI)

 Resources (non used in the current incubation....)
 - timer calculator http://wiki.edwindertien.nl/files/avrtimer.html
 - ATtiny + Arduino http://hlt.media.mit.edu/?p=1695
 - datasheet        http://www.atmel.com/Images/doc2586.pdf
 - servo: https://github.com/fri000/Servo8Bit
 - adafruit raw capturing: https://github.com/adafruit/IR-Commander
 - no-intterupt version https://github.com/TKJElectronics/ATtinyRemote
 - no-intterupt https://github.com/nathanchantrell/TinyPCRemote
 - int0 and timer0 https://github.com/TKJElectronics/ATtinyRemote/blob/master/ATtinyRemoteVer2/ATtinyRemoteVer2.ino
 
 */


#include <Arduino.h>
// --- Two-lead bicolor LED breathing on ATtiny85 ---
// LED between PB0 and PB1 (common anti-parallel type)
// Smooth sine "breathing" between red and green using software PWM

const uint8_t LED1 = 0;  // PB0
const uint8_t LED2 = 1;  // PB1

const float frequency = 0.2; // cycles per second
const float gamma = 2.6;     // brightness shaping

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

// One software PWM cycle
void softwarePWM(uint8_t redValue, uint8_t greenValue) {
  // Each iteration = one full PWM frame (~256 steps)
  for (uint8_t i = 0; i < 255; i++) {
    if (i < redValue) {
      // RED: PB0 high, PB1 low
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
    } else if (i < redValue + greenValue) {
      // GREEN: PB1 high, PB0 low
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
    } else {
      // OFF
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    }
  }
}

void loop() {
  float t = millis() / 1000.0;

  // Two sine waves, opposite phase
  float phaseR = (sin(PI * frequency * t) * 0.5) + 0.5;
  float phaseG = (sin(PI * frequency * t + PI) * 0.5) + 0.5;

  // Nonlinear brightness shaping
  float shapedR = pow(phaseR, gamma);
  float shapedG = pow(phaseG, gamma);

  // Scale to 0–255
  uint8_t redValue = (uint8_t)(shapedR * 255);
  uint8_t greenValue = (uint8_t)(shapedG * 255);

  softwarePWM(redValue, greenValue);
}
