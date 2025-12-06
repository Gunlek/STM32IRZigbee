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

int deviceAddress   = 0xA0;

int status;

int read_write_mode = 0; // 0 = Read, 1 = Write
int parameters[3];
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
        writeData(parameters[0], parameters[1], parameters[2]);
        if(!status) { Serial.println("OK"); }
        else { Serial.println("ERR"); }
      }
      else {
        int value = readData(parameters[0], parameters[1]);
        if(!status) { 
          Serial.print("VAL ");
          Serial.println(value); 
        }
        else { Serial.println("ERR"); }
      }
    }
    newRequest = false;
  }

  // if(bufferSize > 0) {
  //   // --- We have received data
  //   // --- The data should be in the format Page Address Value
  //   // --- 8 bits for each of the value
  //   getParameters(inBuffer, parameters, read_write_mode);

  //   // Serial.println((String)parameters[0] + ", " + parameters[1] + ", " + parameters[2]);

  //   if(read_write_mode == 1) {
  //     writeData(parameters[0], parameters[1], parameters[2]);
  //   }
  //   else {
  //     uint8_t valueRead = readData(parameters[0], parameters[1]);
  //     Serial.print("Read value ");
  //     Serial.print(valueRead);
  //     Serial.print(" on page ");
  //     Serial.print(parameters[0]);
  //     Serial.print(", address ");
  //     Serial.println(parameters[1]);
  //   }
  // }
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
      parametersAcquisition = getParameters(serialInput.substring(1), 3);
    }
    else if(serialInput.startsWith("r")) {
      read_write_mode = 0;
      parametersAcquisition = getParameters(serialInput.substring(1), 2);
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
uint8_t writeData(uint8_t page, uint8_t addr, uint8_t value) {
  if(DEBUG) {
    Serial.println((String)"Writing " + value + " at address " + addr + ", page " + page);
  }
  int writeAddress = ((deviceAddress & 0xF0) >> 1) + page;

  Wire.beginTransmission(writeAddress);
  // --- Send targeted word address
  Wire.write(addr);
  // --- Write desired value
  Wire.write(value);
  // --- End transmission (Send STOP signal)
  status = Wire.endTransmission();

  return status;
}

uint8_t readData(uint8_t page, uint8_t addr) {
  if(DEBUG) {
    Serial.println((String)"Reading on page " + page + ", address " + addr);
  }

  int readAddress = ((deviceAddress & 0xF0) >> 1) + page;

  // --- Position the reading cursor at the correct position
  Wire.beginTransmission(readAddress);
  Wire.write(addr);
  status = Wire.endTransmission();

  int readLength = Wire.requestFrom(readAddress, 1);
  int value = Wire.read();

  // --- Read the value after the cursor
  return value;
}

//  TODO: Add reading function




