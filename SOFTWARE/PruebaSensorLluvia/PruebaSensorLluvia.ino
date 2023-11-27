void setup()
{
  Serial.begin(115200);
}

void loop() {
  Serial.print("Water level Sensor Value:");
  Serial.println(analogRead(25));
  delay(100);
}