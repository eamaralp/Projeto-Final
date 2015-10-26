#include "EmonLib.h"                   // Include Emon Library
#include <SPI.h>
#include <LCD5110_Graph_SPI.h>  

char TIrms[40];
LCD5110 myGLCD(5,6,3);

EnergyMonitor emon1, emon2, emon3;                   // Create an instance

double kwhTotal;
unsigned long ltmillis, tmillis, timems, previousMillis;


extern unsigned char SmallFont[];

void setup()
{  
  Serial.begin(9600);
  myGLCD.InitLCD(70);
  myGLCD.setFont(SmallFont);  
  myGLCD.update();
  emon1.current(0, 80);             // Current: input pin, calibration.
  emon2.current(1, 80);
  emon3.current(2, 80);
}

void loop()
{ 
  double Irms01 = emon1.calcIrms(1480);  // Calcula Corrente
  double Irms02 = emon2.calcIrms(1480);  // Calcula Corrente
  double Irms03 = emon3.calcIrms(1480);  // Calcula Corrente
  double Pot01 = Irms01*111.1; // Calcula Potencia
  double Pot02 = Irms02*111.1; // Calcula Potencia
  double Pot03 = Irms03*111.1; // Calcula Potencia
  
  if(Pot01 > 1 or Pot02 > 1 or Pot03 > 1)
  {
    ltmillis = tmillis;
    tmillis = millis();
    timems = tmillis - ltmillis;
    kwhTotal = kwhTotal + (((Irms01*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
    kwhTotal = kwhTotal + (((Irms02*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
    kwhTotal = kwhTotal + (((Irms03*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
  }


  dtostrf(Irms01, 3, 3, TIrms);
  myGLCD.clrScr();
  myGLCD.print("Kw/h TOTAL:", LEFT, 0);
  myGLCD.print("Potencia(W):", LEFT, 22);
  dtostrf(kwhTotal, 4, 5, TIrms);
  myGLCD.print(TIrms, CENTER, 10);          
  dtostrf(Pot01, 3, 5, TIrms);
  myGLCD.clrLine(0, 34, 80, 45);
  myGLCD.print(TIrms, CENTER, 35);          
  myGLCD.update();
  Serial.print("Irms:");
  Serial.println(TIrms);           // Irms
  
  delay(550);
//  myGLCD.update();

}
