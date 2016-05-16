#include <Arduino.h>
#include "audio.h"
#include "JQ6500.h"
#include "JQ6500_def.h"
#include "def.h"
#include <Wire.h>
#include <AM2321.h>
#include <Adafruit_NeoPixel.h>
//用户自定义部分------------------------
AM2321 am2321;
//用户自定义部分------------------------
#define PIN A0
#define NOTICE A2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel notice = Adafruit_NeoPixel(6, NOTICE, NEO_GRB + NEO_KHZ800);
uint32_t color[9] =
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0), strip.Color(248, 141, 30), strip.Color(255, 255, 0), strip.Color(0, 255, 0), strip.Color(0, 127, 255), strip.Color(0, 0, 255), strip.Color(139, 0, 255), strip.Color(255, 255, 255)
};
//用户自定义部分------------------------
int music_vol = 25;         //初始音量0~30
int music_Mode = MODE_ONE_STOP;
JQ6500 AUDIO(&VOICE);         //声明audio类
//用户自定义部分------------------------
int num = 0;
boolean buffer_sta = false;
boolean start = false;
int brightness = 255;
unsigned long timer_voice = millis();
int flag_on = 0;

int c = 0;
int DHTPIN = 8;
int temperature;
//func===================================
int judge(String cmd);
void writecommand();
void colorWipe(uint32_t c);
void dht11();
void tempsound();
//SETUP===================================
void SETUP_begin()
{
  DEBUG.begin(9600);
  VOICE.begin(9600);
  pinMode(DHTPIN,INPUT);
  //EEPROM---------------------
  config = eeprom_READ();
  music_Mode = config.EEPROM_music_mode;
  music_vol = config.EEPROM_music_vol;
  AUDIO.init(DEVICE_TF, music_Mode, music_vol);
  delay(1000);
//  AUDIO.choose(1);
  AUDIO.pause();
  strip.begin();	//初始化LED
  notice.begin();
  
    notice.setPixelColor(0, strip.Color(0, 0, 0));
    notice.show(); 
  for (int i = 0; i < 6; i++) // 初始化所有灯都为灭
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
  }
// writecommand();    //语音模块配置
}
//LOOP====================================
void LOOP_priority_1ST()
{
  if (millis() < timer_voice) {
    timer_voice = millis();  //同步
  }
  if (millis() - timer_voice < 10000)  //从有配置指令声音输入开始计时
  {
    notice.setPixelColor(0,notice.Color(255, 255, 255));
    notice.show();
  }
  else
  {
    notice.setPixelColor(0,notice.Color(0, 0, 0));
    notice.show();
    flag_on = 0;
  }

  if (VOICE.available())
  {
    String cmd = "";
    if (VOICE.available()) //如果从语音芯片接收到用户的语音输入数据
    {
      cmd += char(VOICE.read()); //读出串口内的内容
      delay(10);
      cmd += char(VOICE.read());
      delay(10);
    }
    Serial.println(flag_on);
    Serial.println(cmd);
    if (cmd == "pp") //如果是语音开关指令对应的内容
    {
      DEBUG.println("Voice IN!");  //在串口提示 输入声音 
      AUDIO.choose(1);
      delay(2000);
      AUDIO.pause();
      flag_on = 1;
      notice.Color(0, 0, 0);
      timer_voice = millis(); //记录当前的系统运行时间
    }
    if (flag_on)   //如果进入了语音输入状态
    {
      c = judge(cmd);
      DEBUG.print("C is :");
      DEBUG.println(c);
      switch (c)  //根据不同的语音指令，选择对应的应答
      {
        case 1:          //名字
          timer_voice = 0;
          AUDIO.choose(4);
          timer_voice = millis();
          break;
        case 2:        //年龄
          timer_voice = 0;
          AUDIO.choose(6);
          timer_voice = millis();
          break;
        case 3:                      //讲故事
          timer_voice = 0;
          AUDIO.choose(3);
          timer_voice = millis();
          delay(9000);     //防止误触发打断
          break;
        case 4:                 //背诗
          timer_voice = 0;
          AUDIO.choose(2);
          delay(3000);
          AUDIO.choose(5);
          delay(10000);
          timer_voice = millis();
          break;
        case 5:                       //天气如何
          timer_voice = 0;
          am2321.read();
          temperature = am2321.temperature/10.0;
          Serial.println(temperature);
          if(temperature<20)AUDIO.choose(15);
          if(temperature>=20&&temperature<27)AUDIO.choose(14);
          if(temperature>=27)AUDIO.choose(16);
          timer_voice = millis();
          break;
        case 6:                     //好黑
          timer_voice = 0;
          AUDIO.choose(9);
          colorWipe(color[8]);
          timer_voice = millis();
          break;
        case 7:                       //再见
          timer_voice = 0;
          AUDIO.choose(11);
          timer_voice = millis();
          break;
        case 8:              //傻瓜
          timer_voice = 0;
          AUDIO.choose(12);
          timer_voice = millis();
          break;
        case 9:                     //关灯
          timer_voice = 0;
          AUDIO.choose(10);         //好的
          colorWipe(color[0]);
          timer_voice = millis();
          break;
        case 10:                  //谢谢
          timer_voice = 0;
          AUDIO.choose(13);        //不客气
          timer_voice = millis();
          break;
        case 0:
          timer_voice = 0;
          AUDIO.pause();
          timer_voice = millis();
          break;
      }
      while (VOICE.available()){
      VOICE.read();}
    }
  }
}


int judge(String cmd)
{
  int c;
  if (cmd == mz)c = 1;
  if (cmd == nl)c = 2;
  if (cmd == gs)c = 3;
  if (cmd == sg)c = 4;
  if (cmd == wd)c = 5;
  if (cmd == kd)c = 6;
  if (cmd == zj)c = 7;
  if (cmd == sm)c = 8;
  if (cmd == gd)c = 9;
  if (cmd == xx)c = 10;
  if (cmd == "pp") {
    c = 0;
  }
  return c;
}

void writecommand()
{
  VOICE.println("{d1}");
  delay(200);
  VOICE.println("{d1}");    //确认进入调试模式
  delay(200);
  VOICE.println("{c0}");      //清除已有指令
  delay(200);
  VOICE.println("{a0pao pao|s0pp}");  //开启语音指令
  delay(200);
  VOICE.println("{a0ni de ming zi|s0mz}"); //你的名字
  delay(200);
  VOICE.println("{a0ni de nian ling|s0nl}");  // 你的年龄
  delay(200);
  VOICE.println("{a0jiang gu shi|s0gs}");   //讲故事
  delay(200);
  VOICE.println("{a0bei shi|s0sg}"); //背诗
  delay(200);
  VOICE.println("{a0sha gua|s0sm}"); //傻瓜
  delay(200);
  VOICE.println("{a0tian qi ru he|s0wd}"); //天气如何
  delay(200);
  VOICE.println("{a0hao hei|s0kd}");  //好黑
  delay(200);
  VOICE.println("{a0guan deng|s0gd}");  //关灯
  delay(200);
  VOICE.println("{a0zai jian|s0zj}");  //再见
  delay(200);
  VOICE.println("{a0xie xie|s0xx}");  //谢谢
  delay(200);
  VOICE.println("{l0}");     //加载
  delay(200);
  VOICE.println("{d0}");    //退出调试
  delay(200);
}
void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

