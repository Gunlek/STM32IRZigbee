#include <Wire.h>

int deviceAddress   = 0xA0 >> 1;
int wordAddress     = 0x00;

int status;

void setup() {
  Serial.begin(115200);

  Wire.begin();

  Serial.println("=== READ OPERATION ===");

  // --- Dummy write request
  Wire.beginTransmission(deviceAddress);
  Wire.write(wordAddress);
  // --- End the transmission before sending any value
  status = Wire.endTransmission();

  Serial.print("Status: ");
  Serial.println(status);

  // --- Request 1 byte to be read
  int returned = Wire.requestFrom(deviceAddress, 4);
  Serial.print("Returned bytes: ");
  Serial.println(returned);

  while(Wire.available()) {
    Serial.print("Value: ");
    Serial.println(Wire.read());
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}




