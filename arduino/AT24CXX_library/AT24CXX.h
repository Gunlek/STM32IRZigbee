/*
  AT24CXX.h
  An Arduino library for the AT24 EEPROM Series.
  Written by Ali Abedi for ROIT CO. August 21, 2018.
  (http://roit.co/ali-abedi)
  The AT24 series of EEPROM are 5v level ICs which use I2C communication protocol to read and write data into them.
  MIT license, all text above must be included in any redistribution.
  */

#ifndef AT24CXX_h
#define AT24CXX_h

#include "Arduino.h"
#include <Wire.h>

class AT24CXX
{
  public:
  //Functions:
    AT24CXX(uint8_t module_code, uint8_t I2C_address);
	void init(void); //put this function in setup() section
	void loadToRAM(int Size); //Size's unit is byte
	uint8_t readFrom(int address);
	void writeTo(int address, uint8_t data);
	void commit(void); //commit all those changed values to EEPROM
  private:
  //Variables:
    int _size;
	uint8_t *_SRAM;
	uint8_t *_EEPROM; //The last and not changed data of EEPROM. (Usage: to be compared with _SRAM data in commit() function)
	int _sizeOfSRAM;
	bool _loadedToRAM;
	uint8_t _EEPROM_I2C_ADDRESS;
};

#endif