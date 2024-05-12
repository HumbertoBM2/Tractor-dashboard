void setup() {
  Serial.begin(115200); // Initialize serial communication
}

void loop() {
  if (Serial.available()) {
    Serial.write(Serial.read()); // Echo incoming data
  }
}
