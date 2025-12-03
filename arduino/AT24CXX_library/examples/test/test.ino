/*
   test.ino - A test code for AT24CXX library
   First connect your AT24 IC to your arduino board. (As described in its data sheet)
   Then compile and upload this code into your arduino board.
   Open serial monitor and type:
  		r[address] -> for reading from an address
  		w[address]:[value] -> for writing a value in an address
  	    commit -> for commiting changes
   This example uses AT24C32A EEPROM IC.
   "Tested on Arduino Due."
   By Ali Abedi (aliabedi@roit.co)
*/
#include <AT24CXX.h>

AT24CXX eeprom(32, 0x50); //all three adrress pins of AT24C32A is connected to GND

void setup() {
  Serial.begin(9600);
  eeprom.init();
  eeprom.loadToRAM(10);
}

void loop() {
  String tmp;
  int addr;
  int value;
  if ( Serial.available() ) {
    tmp = Serial.readString();
    if (tmp.startsWith("r")) {
      addr = (int) tmp.substring(1).toInt();
      Serial.print("-- value in "); Serial.print(addr, DEC); Serial.print(" is : ");
      Serial.println(eeprom.readFrom(addr), DEC);
    } else if (tmp.startsWith("w")) {
      addr = (int) tmp.substring(1, tmp.indexOf(":")).toInt();
      value = (uint8_t) tmp.substring(tmp.indexOf(":") + 1).toInt();
      eeprom.writeTo(addr, value);
      Serial.print("-- Value "); Serial.print(value, DEC); Serial.print(" was written into address "); Serial.println(addr, DEC);
    } else if (tmp.startsWith("commit")) {
      eeprom.commit();
      Serial.println("-- Changes were commited to EEPROM.");
    } else {
      Serial.println("-- Unknown command!");
    }
  }
}
