/**
 * WriteMemoryFromSerial by Fabien AUBRET
 * 28/12/2024
 *
 * This code enable the user to write values to the AT24C08 using simple Serial commands in the following format:
 * page address value
 * With page between 0 and 3
 * Address and value are stored on 8 bits
 *
 * Serial communication speed: 115200 bauds
 *
*/

#include <Wire.h>

int deviceAddress   = 0xA0 >> 1;

int status;

char * inBuffer;
int bufferSize;

int parameters[3];

void setup() {
  Wire.begin();

  Serial.begin(115200);
}

void loop() {
  readSerial();

  if(bufferSize > 0) {
    // --- We have received data
    // --- The data should be in the format Page Address Value
    // --- 8 bits for each of the value
    getParameters(inBuffer, parameters);

    // Serial.println((String)parameters[0] + ", " + parameters[1] + ", " + parameters[2]);

    writeData(parameters[0], parameters[1], parameters[2]);
  }
}

void readSerial() {
  bufferSize = 0;
  if(Serial.available()) {
    int ct = millis();
    while((millis() - ct) < 100) {}
    
    bufferSize = Serial.available();
    inBuffer = malloc(bufferSize);
    int index = 0;

    while(Serial.available() > 0) {
      inBuffer[index] = (char)Serial.read();
      index++;
    }
  }
}

/*
 * Extract parameters from a string in the following format: X Y Z (space separated)
 */
void getParameters(char * buffer, int * parameters) {
  String inStr = String(buffer);

  // --- Reading first parameter
  uint8_t delim1 = inStr.indexOf(" ");
  parameters[0] = inStr.substring(0, delim1).toInt();
  inStr = inStr.substring(delim1+1, inStr.length());

  // --- Reading second parameter
  uint8_t delim2 = inStr.indexOf(" ");
  parameters[1] = inStr.substring(0, delim2).toInt();
  inStr = inStr.substring(delim2+1, inStr.length());

  // --- Reading last parameter
  parameters[2] = inStr.toInt();
}

/*
 * page: uint8_t, 0, 1, 2, 3
 * addr: uint8_t, the address to write the byte to
 * value: uint8_t, the value (byte) to be written
 */
uint8_t writeData(uint8_t page, uint8_t addr, uint8_t value) {
    Serial.println((String)"Writing " + value + " at address " + addr);

    Wire.beginTransmission(deviceAddress);
    // --- Send targeted word address
    Wire.write(addr);
    // --- Write desired value
    Wire.write(value);
    // --- End transmission (Send STOP signal)
    status = Wire.endTransmission();

    if(status == 0) {
      Serial.println("Byte written successfully");
    }
    else {
      Serial.println((String)"Error while writing the value: " + status);
    }
}

//  TODO: Add reading function




