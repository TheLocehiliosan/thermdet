void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(3) == HIGH) {
    Serial.print("HIGH\n");
  }
  else {
    Serial.print("LOW\n");
  }
  delay(250);
}
