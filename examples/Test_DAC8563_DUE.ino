#include "DAC8562_DUE.h"

#define REF_POWER 3.3383

//CLR and LDAC = GND

//Arduino Leonardo (SCLK,DIN,CS) = SCK,MOSI,10 - NOT WORKING HARDWARE SPI
//Arduino UNO (SCLK,DIN,CS) = 13,11,10
#define MSG   "Using Hardware SPI"

unsigned int SPI_CS = 10;    //PIN SYNC on DAC8563 Board

#define DAC_MIN   512 
#define DAC_MAX   65024

DAC8562 dac=DAC8562(SPI_CS);

void setup() {
  //begin(byte SCK_, byte MOSI_, byte MISO_, byte SS_)
  delay(1500);
  dac.begin();
  delay(100);
  SerialUSB.begin(115200);
  delay(500);
  SerialUSB.println(MSG);  
  SerialUSB.println("Enter value: (-255:255).");
}

void Set_DAC_A(int16_t val)
{
  dac.outPutValue(CMD_SETA_UPDATEA,val);
  
}

void Set_DAC_B(int16_t val)
{
  dac.outPutValue(CMD_SETB_UPDATEB,val);
  
}


void loop() {
  char buff[64];
  // put your main code here, to run repeatedly:
volatile int force =0;
volatile int DACValue=0;
volatile float DACVoltage =0;
  while(SerialUSB.available()>0)
    {
      SerialUSB.setTimeout(100);
      force = SerialUSB.parseInt();
       DACValue = map(force,-255,255,DAC_MAX,DAC_MIN);       
        
      sprintf(buff,"force = %d, DACValue = %d", force,DACValue);
      SerialUSB.println(buff);
        
     Set_DAC_A(DACValue);
     Set_DAC_B(-DACValue);
    
      //dac.writeA(DACVoltage);
      //dac.writeB(DACVoltage);
      
    }
  
  
}
