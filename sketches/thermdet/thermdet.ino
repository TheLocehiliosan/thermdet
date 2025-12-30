// PINS
// avoid 0 & 1 to preserve serial communication
// 5 is special on the ItsyBitsy (high voltage output only)
#define PIN_LED_TOP      A0
#define PIN_LED_LEFT     A1
#define PIN_LED_MIDDLE   A2
#define PIN_LED_RIGHT    A3
#define PIN_TRIGGER       3
#define PIN_SND_RST       7
#define PIN_SND_ACTIVITY  9
#define PIN_SND_ARM      10
#define PIN_SND_LOOP     11
#define PIN_SND_DISARM   12

const int ledPins[] = {PIN_LED_TOP, PIN_LED_LEFT, PIN_LED_MIDDLE, PIN_LED_RIGHT};
const int sndPins[] = {PIN_SND_ARM, PIN_SND_LOOP, PIN_SND_DISARM};

enum triggerState {armed, disarmed};
enum triggerState last_tstate = disarmed;
enum triggerState this_tstate = disarmed;

void setup() {
  Serial.begin(9600);
  // set up LEDs
  for (int pin: ledPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  // set up sound
  pinMode(PIN_SND_RST, OUTPUT);
  digitalWrite(PIN_SND_RST, LOW);
  pinMode(PIN_SND_ACTIVITY, INPUT_PULLUP);
  for (int pin: sndPins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
  digitalWrite(PIN_SND_RST, HIGH);

  // set up trigger
  pinMode(PIN_TRIGGER, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(PIN_TRIGGER) == HIGH) {
    this_tstate = disarmed;
  }
  else {
    this_tstate = armed;
  }
  if (this_tstate != last_tstate) {
    if (this_tstate == armed) {
      Serial.print("Armed\n");
    }
    else {
      Serial.print("Disarmed\n");
    }
  }
  delay(10);
  last_tstate = this_tstate;
}
