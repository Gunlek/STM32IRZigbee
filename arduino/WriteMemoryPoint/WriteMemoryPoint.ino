#include <Wire.h>

int deviceAddress   = 0xA0 >> 1;
int wordAddress     = 0x00;
int writeValue      = 120;

int status;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  Serial.println("=== WRITE OPERATION ===");
  // --- Start the transmission to the device address
  Wire.beginTransmission(deviceAddress);
  // --- Send targeted word address
  Wire.write(wordAddress);
  // --- Write desired value
  Wire.write(writeValue);
  // --- End transmission (Send STOP signal)
  status = Wire.endTransmission();
  
  Serial.print("Status: ");
  Serial.println(status);

  if(status == 0) {
    Serial.print("Wrote byte ");
    Serial.print(writeValue);
    Serial.println(" Successfully");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}




