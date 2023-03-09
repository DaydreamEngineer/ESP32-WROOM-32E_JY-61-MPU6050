#include "Arduino.h"
#include <Wire.h>

#define RXD2 16
#define TXD2 17

int nDataCnt = 0;
char buffer[11];

void setup() 
{
  Serial.begin(115200);  
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);  
}

void loop() 
{
  while(Serial2.available())
  {
    buffer[nDataCnt] = Serial2.read();
    if(nDataCnt == 0 && buffer[0]!= 0x55)
    {
      return;
    }
    nDataCnt++;
    if(nDataCnt ==11) // 讀滿11筆資料
    {
      nDataCnt = 0;      
      HandleData(buffer); //解讀資料
    }
  }
}


void HandleData(char buffer[])
{
  float a[3], w[3], angle[3], T; // a:三軸加速度, w:角速度, angle:角度, T:溫度
  if(buffer[0]==0x55) //資料Header
  {
    //分辨哪類資料
    switch(buffer[1]) 
    {
      case 0x51: //加速度
        a[0] = (short(buffer [3]<<8| buffer [2]))/32768.0*16;
        a[1] = (short(buffer [5]<<8| buffer [4]))/32768.0*16;
        a[2] = (short(buffer [7]<<8| buffer [6]))/32768.0*16;
        T = (short(buffer [9]<<8| buffer [8]))/340.0+36.53;

      break;

      case 0x52:
        w[0] = (short(buffer [3]<<8| buffer [2]))/32768.0*2000;
        w[1] = (short(buffer [5]<<8| buffer [4]))/32768.0*2000;
        w[2] = (short(buffer [7]<<8| buffer [6]))/32768.0*2000;
        T = (short(buffer [9]<<8| buffer [8]))/340.0+36.53;

      break;

      case 0x53:
        angle[0] = (short(buffer [3]<<8| buffer [2]))/32768.0*180;
        angle[1] = (short(buffer [5]<<8| buffer [4]))/32768.0*180;
        angle[2] = (short(buffer [7]<<8| buffer [6]))/32768.0*180;
        T = (short(buffer [9]<<8| buffer [8]))/340.0+36.53;

        // 舉例印出三軸現在角度讀值
        Serial.print("X:");
        Serial.print(angle[0]);
        Serial.print(", Y:");
        Serial.print(angle[1]);
        Serial.print(", Z:");
        Serial.println(angle[2]);

      break; 
    }
  }
}




