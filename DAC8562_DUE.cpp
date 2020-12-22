/*
  DAC8562.cpp
  
  Arduino library for Texas Instruments DAC8562 2-channel 16-bit SPI DAC
  
  2018-07-24 @km7,  (cc) https://creativecommons.org/licenses/by/3.0/
*/

#include "DAC8562_DUE.h"

DAC8562::DAC8562()
{
  _cs_pin = 10;
  _vref=3.3383; //My Board using vref 3.3V
};

DAC8562::DAC8562(uint8_t cs_pin)
{
  _cs_pin = cs_pin;
  _vref=3.3383; //My Board using vref 3.3V
};

DAC8562::DAC8562( uint8_t cs_pin, float vref)
{
  _cs_pin = cs_pin;
  _vref=vref;
};

void DAC8562::begin()
{
  SPI.begin(_cs_pin);
  SPI.setDataMode(_cs_pin,SPI_MODE1);
  SPI.setBitOrder(_cs_pin,MSBFIRST);
  initialize();
  /* !Chip select (low to enable) */
  //pinMode(_cs_pin, OUTPUT);
  //digitalWrite(_cs_pin,  1);
};


void DAC8562::DAC_WR_REG(uint8_t cmd_byte, uint16_t data_byte) {
  //digitalWrite(_cs_pin, 0);
  SPI.transfer(_cs_pin,cmd_byte,SPI_CONTINUE);
  SPI.transfer16(_cs_pin,data_byte);
  //digitalWrite(_cs_pin, 1);
};


void DAC8562::outPutValue(uint8_t cmd_byte,uint16_t input) {
  byte inputMid = (input>>8)&0xFF;
  byte inputLast = input&0xFF;
  unsigned int  t= (input>>8)&0xFF;
  writeValue(cmd_byte, (inputLast),(inputMid));
};

void DAC8562::writeVoltage(float input) {
  writeA(input);
  writeB(input);
};

void DAC8562::writeA(float input) {
 outPutValue(CMD_SETA_UPDATEA,Voltage_Convert(input/_vref*5));
};

void DAC8562::writeB(float input) {
 outPutValue(CMD_SETB_UPDATEB,Voltage_Convert(input/_vref*5));
};

void DAC8562::writeValue(uint8_t cmd_byte, uint8_t mid, uint8_t last) {
  //digitalWrite(_cs_pin, 0);
  SPI.transfer(_cs_pin,cmd_byte,SPI_CONTINUE);
  SPI.transfer(_cs_pin,last,SPI_CONTINUE);
  SPI.transfer(_cs_pin,mid);
  
  //digitalWrite(_cs_pin, 1);
};

void DAC8562::initialize() {
  DAC_WR_REG(CMD_RESET_ALL_REG, DATA_RESET_ALL_REG);      // reset
  DAC_WR_REG(CMD_PWR_UP_A_B, DATA_PWR_UP_A_B);        // power up
  DAC_WR_REG(CMD_INTERNAL_REF_EN, DATA_INTERNAL_REF_EN);      // enable internal reference
  DAC_WR_REG(CMD_GAIN, DATA_GAIN_B2_A2);            // set multiplier
  DAC_WR_REG(CMD_LDAC_DIS, DATA_LDAC_DIS);          // update the caches
};

uint16_t DAC8562::Voltage_Convert(float voltage)
{
  uint16_t _D_;
  
  voltage = voltage / 6  + 2.5;   //based on the manual provided by texas instruments

  _D_ = (uint16_t)(65536 * voltage / 5);

  if(_D_ < 32768)
  {
    _D_ -= 100;     //fix the errors
  }
    
  return _D_;
};