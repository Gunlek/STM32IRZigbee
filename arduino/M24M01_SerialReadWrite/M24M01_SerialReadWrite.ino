/**
 * WriteMemoryFromSerial by Fabien AUBRET
 * 28/12/2024
 * 03/12/2025 ==> Adding reading capabilities
 *
 * This code enable the user to write values to the AT24C08 using simple Serial commands in the following format:
 * page address value
 * For example:
 *  To read page 0, address 0:
 *    r0 0
 *  To write page 0, address 0, value 58:
 *    w0 0 58
 * With page between 0 and 3
 * Address and value are stored on 8 bits
 *
 * Serial communication speed: 115200 bauds
 *
*/

#include <Wire.h>

uint8_t addressBase   = 0xA0;
uint8_t e1Flag        = 0x08;
uint8_t e2Flag        = 0x04;

uint8_t deviceAddress = addressBase | e1Flag | e2Flag; // 1010 e1 e2 00

int status;

int read_write_mode = 0; // 0 = Read, 1 = Write
uint32_t parameters[2];
bool parametersAcquisition;

bool newRequest = false;

String serialInput;

bool DEBUG = false;

void setup() {
  Wire.begin();

  Serial.begin(115200);
}

void loop() {
  readSerial();

  if(newRequest) {
    if(!parametersAcquisition) {
      Serial.println("ERR");
    }
    else {
      if(read_write_mode == 1) {
        writeData(parameters[0], parameters[1]);
        if(!status) { Serial.println("OK"); }
        else { Serial.println("ERR"); }
      }
      else {
        int value = readData(parameters[0]);
        if(!status) { 
          Serial.print("VAL ");
          Serial.println(value); 
        }
        else { Serial.println("ERR"); }
      }
    }
    newRequest = false;
  }
}

void readSerial() {
  if(Serial.available()) {
    newRequest = true;
    serialInput = Serial.readStringUntil('\n');
    serialInput.replace("\r", "");
    serialInput.trim();
    int index = 0;

    if(serialInput.startsWith("w")) {
      read_write_mode = 1;
      parametersAcquisition = getParameters(serialInput.substring(1), 2);
    }
    else if(serialInput.startsWith("r")) {
      read_write_mode = 0;
      parametersAcquisition = getParameters(serialInput.substring(1), 1);
    }
    else {
      Serial.println("ERR");
    }
  }
}

/*
 * Extract parameters from a string in the following format: X Y Z (space separated)
 */
bool getParameters(String input, int paramCount) {
  int index = 0;

  while(input.length() > 0 && index < paramCount) {
    int spaceIndex = input.indexOf(" ");

    if(spaceIndex >= 0) {
      parameters[index] = input.substring(0, spaceIndex).toInt();
      input = input.substring(spaceIndex+1);
      input.trim();
      index++;
    }
    else {
      parameters[index] = input.toInt();
      index++;
      break;
    }
  }

  return (index == paramCount);
}

/*
 * page: uint8_t, 0, 1, 2, 3
 * addr: uint8_t, the address to write the byte to
 * value: uint8_t, the value (byte) to be written
 */
uint8_t writeData(uint32_t addr, uint8_t value) {
  if(DEBUG) {
    Serial.println((String)"Writing " + value + " at address " + addr);
  }
  uint8_t byteAddrMSB = (addr >> 9 & 0x80) >> 6;
  uint8_t deviceWriteAddress = (deviceAddress | byteAddrMSB) >> 1;
  uint8_t writeAddressUp = addr >> 8 & 0xFF;
  uint8_t writeAddressDown = addr & 0xFF;

  Wire.beginTransmission(deviceWriteAddress);
  // --- Send targeted word address
  Wire.write(writeAddressUp);
  Wire.write(writeAddressDown);
  // --- Write desired value
  Wire.write(value);
  // --- End transmission (Send STOP signal)
  status = Wire.endTransmission();

  return status;
}

uint8_t readData(uint32_t addr) {
  if(DEBUG) {
    Serial.println((String)"Reading address " + addr);
  }

  if(addr <= 128000) {
    uint8_t byteAddrMSB = (addr >> 9 & 0x80) >> 6;
    uint8_t deviceReadAddress = (deviceAddress | byteAddrMSB) >> 1;
    uint8_t readAddressUp = addr >> 8 & 0xFF;
    uint8_t readAddressDown = addr & 0xFF;

    // --- Position the reading cursor at the correct position
    Wire.beginTransmission(deviceReadAddress);
    Wire.write(readAddressUp);
    Wire.write(readAddressDown);
    status = Wire.endTransmission();

    int readLength = Wire.requestFrom(deviceReadAddress, 1);
    int value = Wire.read();

    // // --- Read the value after the cursor
    return value;
  }
  else {
    return 0;
  }
}

//  TODO: Add reading function




