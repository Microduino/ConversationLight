#include "Micromenu_user.h"
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <AM2321.h>
//EEPROM---------------------
#include <EEPROM.h>

//用户自定义部分------------------------
#include <SoftwareSerial.h>


void setup() 
{
  SETUP_begin();     //该函数定义于 Micromenu_user.h
  //用户自定义部分========================
}


void loop() 
{
  {
    LOOP_priority_1ST(); //该函数定义于 Micromenu_user.h
  }
}
