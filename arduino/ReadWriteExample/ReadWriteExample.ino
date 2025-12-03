#include <Wire.h>

#define readWriteMask 0x01
#define addrMask 0xF0

int deviceAddress = 0xA0;
int writeFlag = 0;        // 0: Write, 1: Read

int wordAddress = 0x00;   // Word address, on 8 bits
int writeValue = 0xFA;
int status;

void setup() {
  Serial.begin(9600);

  Wire.begin();

  Serial.println("=== WRITE OPERATION ===");
  // --- Dummy write to the wordAddress - 1
  // --- We want to read 0x00 ==> We dummy write 0xFF and the counter will loop
  Wire.beginTransmission((deviceAddress & 0xF0) >> 1);
  Wire.write(wordAddress);
  Wire.write(writeValue);
  status = Wire.endTransmission();
  Serial.print("Status: ");
  Serial.println(status);

  if(status == 0) {
    Serial.print("Wrote byte ");
    Serial.print(writeValue);
    Serial.println(" Successfully");
  }

  delay(500);

  Serial.println("=== READ OPERATION ===");
  Wire.beginTransmission((deviceAddress & 0xF0) >> 1);
  Wire.write(wordAddress);
  status = Wire.endTransmission();

  Serial.print("Status: ");
  Serial.println(status);
  int returned = Wire.requestFrom((deviceAddress & 0xF0) >> 1, 1);
  Serial.print("Returned bytes: ");
  Serial.println(returned);
  Serial.print("Value: ");
  Serial.println(Wire.read());


  // Wire.requestFrom((deviceAddress & 0xF0) >> 1, 1);
  // Serial.print("Value: ");
  // Serial.println(Wire.read());



  // Wire.beginTransmission((deviceAddress & 0xF0) >> 1);
  // Serial.print("Available: ");
  // Serial.println(Wire.available());

  
  


}

void loop() {
  // put your main code here, to run repeatedly:

}
