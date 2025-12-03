/*
  AT24CXX.h
  An Arduino library for the AT24 EEPROM Series.
  Written by Ali Abedi for ROIT CO. August 21, 2018. Updated in August 29, 2018.
  (http://roit.co/ali-abedi)
  The AT24 series of EEPROM are 5v level ICs which use I2C communication protocol to read and write data into them.
  MIT license, all text above must be included in any redistribution.
  */

#include "Arduino.h"
#include "AT24CXX.h"

AT24CXX::AT24CXX(uint8_t ic_code, uint8_t I2C_address)
{
	_size = ic_code;
	_EEPROM_I2C_ADDRESS = I2C_address;
}

void AT24CXX::init(void)
{
	Wire.begin();
	_sizeOfSRAM = 0;
}

void AT24CXX::loadToRAM(int Size)
{
	if(Size <= _size * 128){
		//_SRAM = malloc( Size * sizeof(uint8_t) );
		_SRAM = (uint8_t *) malloc((size_t) Size);
		_EEPROM = (uint8_t *) malloc((size_t) Size);
		_sizeOfSRAM = Size;
		for(int i = 0; i < Size; i++){
			_SRAM[i] = readFrom(i);
			_EEPROM[i] = _SRAM[i];
		}
		_loadedToRAM = true;
	}
}

uint8_t AT24CXX::readFrom(int address)
{
	if ( _loadedToRAM && (address < _sizeOfSRAM) ) {
		return _SRAM[address];
	} else {
		if (address < _size * 128) {
			uint8_t tmpData = 0xFF;
			Wire.beginTransmission(_EEPROM_I2C_ADDRESS);
			Wire.write((int)(address >> 8));   // MSB
			Wire.write((int)(address & 0xFF)); // LSB
			Wire.endTransmission();
			Wire.requestFrom(_EEPROM_I2C_ADDRESS, 1);
			tmpData =  Wire.read();
			return tmpData;
		}
	}
}

void AT24CXX::writeTo(int address, uint8_t data)
{
	if ( _loadedToRAM && (address < _sizeOfSRAM) ) {
		_SRAM[address] = data;
	} else {
		if (address < _size * 128) {
			Wire.beginTransmission(_EEPROM_I2C_ADDRESS);
			Wire.write((int)(address >> 8));   // MSB
			Wire.write((int)(address & 0xFF)); // LSB
			Wire.write(data);
			Wire.endTransmission();
			delay(10); //write cycle delay
		}
	}
}

void AT24CXX::commit(void)
{
	if ( _loadedToRAM ) {
		_loadedToRAM = false;
		for(int i = 0; i < _sizeOfSRAM; i++){
			if(_EEPROM[i] != _SRAM[i]){
				_EEPROM[i] = _SRAM[i];
				writeTo(i, _EEPROM[i]);
			}
		}
		_loadedToRAM = true;
	}
	
}
