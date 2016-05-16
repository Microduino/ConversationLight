#include <Arduino.h>


#include <SoftwareSerial.h>
//定义蓝牙通信串口
//SoftwareSerial mySerial(4, 5); // RX, TX

//定义串口
//#define FROM mySerial    
#define VOICE Serial1
#define DEBUG Serial

String mz = "mz";
String nl = "nl";
String gs = "gs";
String sg = "sg";
String wd = "wd";
String kd = "kd";
String zj = "zj";
String sm = "sm";
String gd = "gd";
String xx = "xx";
String zl = "zl";

int led_style=2;
int led_style_vol=2;
//int led_style_cache=0;

boolean led_sta=true;
//定义亮度
#define led_brightness  3
unsigned long led_time;


