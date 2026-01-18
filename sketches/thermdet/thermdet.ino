#include <ALib0.h>

// PINS
// avoid 0 & 1 to preserve serial communication
// 5 is special on the ItsyBitsy (high voltage output only)
#define PIN_LED_TOP      A0
#define PIN_LED_LEFT     A1
#define PIN_LED_MIDDLE   A2
#define PIN_LED_RIGHT    A3
#define PIN_TRIGGER       9
#define PIN_SND_ARM      10
#define PIN_SND_LOOP     11
#define PIN_SND_DISARM   12

AButton trigger(PIN_TRIGGER);

const int ledPins[] = {PIN_LED_TOP, PIN_LED_LEFT, PIN_LED_MIDDLE, PIN_LED_RIGHT};
const int sndPins[] = {PIN_SND_ARM, PIN_SND_LOOP, PIN_SND_DISARM};

boolean armed = false;

void setup() {
  // set up LEDs
  for (int pin: ledPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  // set up sound
  for (int pin: sndPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // set up trigger
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
  armed = !digitalRead(PIN_TRIGGER);
}

// watch the trigger, and set a global state variable
void watch_trigger() {
  taskBegin();
  if (trigger.changed()) {
    armed = !digitalRead(PIN_TRIGGER);
  }
  taskEnd();
}

// when armed, play 0(arm) followed by 1(loop)
// when disarmed, play 3(disarm)
void make_noise() {
  taskBegin();
  taskWaitFor(armed);
  digitalWrite(PIN_SND_ARM, LOW); taskDelay(300); digitalWrite(PIN_SND_ARM, HIGH);
  digitalWrite(PIN_SND_LOOP, LOW);
  taskWaitFor(!armed);
  digitalWrite(PIN_SND_LOOP, HIGH);
  digitalWrite(PIN_SND_DISARM, LOW); taskDelay(300); digitalWrite(PIN_SND_DISARM, HIGH);
  taskEnd();
}

// use yellow LEDs as a digital counter
// the counter will slowing increase in speed (to a set maximum)
void counter() {
  static float speed = 1.0;
  static int   count = 0;
  taskBegin();
  if (!armed) {
    count = 0;
    speed = 1.0;
    digitalWrite(PIN_LED_LEFT,   LOW);
    digitalWrite(PIN_LED_MIDDLE, LOW);
    taskRestart();
  }
  count++;
  if (count > 3) {
    count = 1;
  }
  digitalWrite(PIN_LED_LEFT,   bitRead(count, 0) == 1);
  digitalWrite(PIN_LED_MIDDLE, bitRead(count, 1) == 1);
  taskDelay(1000 * (1.0/speed));
  digitalWrite(PIN_LED_LEFT,   LOW);
  digitalWrite(PIN_LED_MIDDLE, LOW);
  if (speed <= 7.0) {
    speed += 0.1;
  }
  taskEnd();
}


// the right red LED will blink slower, and forever speed up
void warning_light() {
  static float speed = 1.0;
  taskBegin();
  if (!armed) {
    speed = 1.0;
    digitalWrite(PIN_LED_RIGHT, LOW);
    taskRestart();
  }
  taskDelay(3500 * (1.0/speed));
  digitalWrite(PIN_LED_RIGHT, !digitalRead(PIN_LED_RIGHT));
  speed += 0.5;
  taskEnd();
}

// the power light will randomly blink for a random amount of time
void power_light() {
  taskBegin();
  if (!armed) {
    digitalWrite(PIN_LED_TOP, LOW);
    taskRestart();
  }
  digitalWrite(PIN_LED_TOP, random(100) > 50);
  taskDelay(random(3000));
  taskEnd();
}

void loop() {
  watch_trigger();
  make_noise();
  counter();
  warning_light();
  power_light();
}
