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

char * inBuffer;
int bufferSize;

int read_write_mode = 0; // 0 = Read, 1 = Write

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
    getParameters(inBuffer, parameters, read_write_mode);

    // Serial.println((String)parameters[0] + ", " + parameters[1] + ", " + parameters[2]);

    if(read_write_mode == 1) {
      writeData(parameters[0], parameters[1], parameters[2]);
    }
    else {
      uint8_t valueRead = readData(parameters[0], parameters[1]);
      Serial.print("Read value ");
      Serial.print(valueRead);
      Serial.print(" on page ");
      Serial.print(parameters[0]);
      Serial.print(", address ");
      Serial.println(parameters[1]);
    }
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
      if(index == 0) {
        switch((char)Serial.read()) {
          case 'r':
            read_write_mode = 0;
            break;

          case 'w':
            read_write_mode = 1;
            break;
          
          default:
            while(Serial.available() > 0) { Serial.read(); }
            Serial.println("[Error] Missing or incorrect w/r flag at the beginning of the command");
            break;
        }
      }
      else {
        inBuffer[index - 1] = (char)Serial.read();
      }
      index++;
    }
  }
}

/*
 * Extract parameters from a string in the following format: X Y Z (space separated)
 */
void getParameters(char * buffer, int * parameters, int mode) {
  String inStr = String(buffer);

  // --- Reading first parameter
  uint8_t delim1 = inStr.indexOf(" ");
  parameters[0] = inStr.substring(0, delim1).toInt();
  inStr = inStr.substring(delim1+1, inStr.length());

  // --- Reading second parameter
  uint8_t delim2 = inStr.indexOf(" ");
  parameters[1] = inStr.substring(0, delim2).toInt();
  inStr = inStr.substring(delim2+1, inStr.length());

  if(mode) {
    // --- Reading last parameter
    parameters[2] = inStr.toInt();
  }
}

/*
 * page: uint8_t, 0, 1, 2, 3
 * addr: uint8_t, the address to write the byte to
 * value: uint8_t, the value (byte) to be written
 */
uint8_t writeData(uint8_t page, uint8_t addr, uint8_t value) {
    Serial.println((String)"Writing " + value + " at address " + addr);
    int writeAddress = ((deviceAddress & 0xF0) >> 1) | page << 1;

    Wire.beginTransmission(writeAddress);
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

uint8_t readData(uint8_t page, uint8_t addr) {
  Serial.println((String)"Reading on page " + page + ", address " + addr);

  int readAddress = ((deviceAddress & 0xF0) >> 1) | page << 1;

  // --- Position the reading cursor at the correct position
  Wire.beginTransmission(readAddress);
  Wire.write(addr);
  status = Wire.endTransmission();

  int readLength = Wire.requestFrom(readAddress, 1);
  int value = Wire.read();

  // --- Read the value after the cursor
  return value;
  // return Wire.requestFrom((deviceAddress & 0xF0) >> 1, 1);
}

//  TODO: Add reading function




