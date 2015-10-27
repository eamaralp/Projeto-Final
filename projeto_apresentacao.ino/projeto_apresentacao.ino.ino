#include "EmonLib.h"                   // Include Emon Library
#include <SPI.h>
#include "String.h"
#include <LCD5110_Graph_SPI.h>  

char irmsAux[40];
char aux01[20], aux02[20], aux03[20];
String strg01, strg02, strg03, strgTotal;
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
  // Calcula Corrente
  double irms01 = emon1.calcIrms(1480);  
  double irms02 = emon2.calcIrms(1480);  
  double irms03 = emon3.calcIrms(1480); 
  // Calcula Potencia
  double pot01 = irms01*111.1; 
  double pot02 = irms02*111.1; 
  double pot03 = irms03*111.1; 

  // Valores abaixo de 1 watt não sao computados, o calculo realizado é o de kw/w
  // os valores são acumulados em uma única variável para apresentar o kw/h total
  if(pot01 > 1 or pot02 > 1 or pot03 > 1)
  {
    ltmillis = tmillis;
    tmillis = millis();
    timems = tmillis - ltmillis;
    kwhTotal = kwhTotal + (((irms01*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
    kwhTotal = kwhTotal + (((irms02*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
    kwhTotal = kwhTotal + (((irms03*111.1)/1000.0) * 1.0/3600.0 * (timems/1000.0)); 
  }
  // Para a apresentação visual valores menores que 2 watts não sao apresentados no display
  if(pot01 < 2)
  {
    pot01 = 0;  
  }
  if(pot02 < 2)
  {
    pot02 = 0;  
  }
  if(pot03 < 2)
  {
    pot03 = 0;  
  }
  
  myGLCD.clrScr(); // limpando o diplay lcd

  // convertendo a corrente para string e preparando para apresentar no display
  myGLCD.print("Sensor1.....", LEFT, 0);;
  dtostrf(pot01, 4, 0, irmsAux);
  strg01 = irmsAux;
  strg01 += "W";
  myGLCD.print(strg01, RIGHT, 0);
  
  myGLCD.print("Sensor2.....", LEFT, 10);
  dtostrf(pot02, 4, 0, irmsAux);
  strg02 = irmsAux;
  strg02 += "W";
  myGLCD.print(strg02, RIGHT, 10);
  
  myGLCD.print("Sensor3.....", LEFT, 21);
  dtostrf(pot03, 4, 0, irmsAux);
  strg03 = irmsAux;
  strg03 += "W";
  myGLCD.print(strg03, RIGHT, 21);
  
  myGLCD.print("T:", LEFT, 38);
  dtostrf(kwhTotal, 4, 1, irmsAux);
  strgTotal = irmsAux;
  strgTotal += " Kw/h";
  myGLCD.print(strgTotal, RIGHT, 38);          
          
  myGLCD.update();
  
  delay(550);

}
