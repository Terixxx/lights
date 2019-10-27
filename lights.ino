/*
  Версия 8 със закоментирано пресмятане на консумацията и изключването на филтъра по време на хранене
  Добавена РН сонда
*/
//#define EXT_FILTER_PIN 17
//#define EXT_AIR_PIN 15
//#define EXT_HEATER_PIN 16
//#define EXT_COOLING_PIN 11
//#define EXT_LIGHT_1_PIN 12
//#define EXT_LIGHT_2_PIN 13
//#define EXT_CO2_PIN 14
//#define VIN_PIN 1
//#define ALARM_PIN 0
//#define EXT_POWER_ON_PIN 17 //на платката е A3
#define PWM1_PIN 10
#define PWM2_PIN 11
#define PWM3_PIN 9

//int analogPin = 19;
//float reperValue = 0;

#define Key_PIN A2
String allow = " ";
byte LED1_TIME = 0;
byte LED2_TIME = 0;
byte LED3_TIME = 0;
byte LEDMoon_TIME = 0;
byte max1PWM = 0;
byte max2PWM = 0;
byte max3PWM = 0;
byte maxMoonPWM = 0;
boolean LEDMoon_On = true;
//byte minPWM = 0;
int cur1PWM = 0;
int cur2PWM = 0;
int cur3PWM = 0;
int curMoonPWM = 0;

#include <LCD5110_Graph.h>

#define BACK_LIGHT_PIN 3
LCD5110 myGLCD(8, 7, 6, 4, 5);
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
//extern uint8_t term[];
byte LCDContrast;
byte LCDLight;

#include <EEPROM.h>

//#include <OneWire.h>
//OneWire  temp(19);  // on pin 10 (a 4.7K resistor is necessary)
//float cur_temp = -999;
//byte temp_type_s;
//byte temp_data[12];
//byte temp_addr[8];
//byte temp_cicle = 0;
// I assume you know how to connect the DS1307.
// DS1307:  SDA pin   -> Arduino Digital 4
//          SCL pin   -> Arduino Digital 5

#include <DS1307.h>

// Init the DS3231
DS1307 rtc(13, 12);
// Init a Time-data structure
Time  time;
unsigned long currentTime;
unsigned long loopTime;
byte key = 0;
byte prev_key = 0;
byte idle_key = 0;

#define max_idle_key  200 //след колко време да се върне в основното меню 100=25сек

int cur_menu = 0;
int cur_item = 0;
boolean cur_item_edit = false;
int aquaT;
//boolean CoolingEnabled;
//boolean light1 = false;
//boolean light11 = false;
//boolean light12 = true;
//boolean light2 = false;
//boolean light21 = false;
//boolean light22 = true;
//boolean filter = true;
//boolean fil = true;
//boolean CO2 = false;
//boolean CO2_1 = false;
//boolean CO2_2 = true;
//boolean AIR = false;
//boolean PH_sonda = false;    // има ли РН сонда
boolean LED1_start = false;
boolean LED2_start = false;
boolean LED3_start = false;
boolean LED1_Man_On = false;
boolean LED2_Man_On = false;
boolean LED3_Man_On = false;
boolean LEDMoon_start = false;
byte at;
byte prSec;

//byte minCool = 0;
//byte maxCool = 0;

//-------------Запис и четене на данни float в EEPROM-------------------------------
void EEPROM_float_write(int addr, float val) // запись в ЕЕПРОМ
{
  byte *x = (byte *)&val;
  for (byte i = 0; i < 4; i++) EEPROM.write(i + addr, x[i]);
}
float EEPROM_float_read(int addr) // чтение из ЕЕПРОМ
{
  byte x[4];
  for (byte i = 0; i < 4; i++) x[i] = EEPROM.read(i + addr);
  float *y = (float *)&x;
  return y[0];
}

float y1 ;//= EEPROM_float_read(60);
float y2 ;//= EEPROM_float_read(65);

//-------------------------------------------------------------------


//int EEPROM_int_read(int addr) {
//  byte raw[2];
//  for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr + i);
//  int &num = (int&)raw;
//  return num;
//}
//
//
//void EEPROM_int_write(int addr, int num) {
//  byte raw[2];
//  (int&)raw = num;
//  for (byte i = 0; i < 2; i++) EEPROM.write(addr + i, raw[i]);
//}
//int x1 ;
//int x2 ;

//----------------------------------------------------------------------------------


void setup()
{
// EEPROM.write(200 + 4, 11);
//  EEPROM.write (7, 2);
  //rtc.setTime(22, 10, 10);
//  Serial.begin(9600);
//  Serial.print("EEPROM size: ");
//  Serial.println(EEPROM.length());
  //  first_setrup();
  //  pinMode(EXT_HEATER_PIN, OUTPUT);
  //  digitalWrite(EXT_HEATER_PIN, LOW);
  //  pinMode(EXT_COOLING_PIN, OUTPUT);
  //  digitalWrite(EXT_COOLING_PIN, LOW);
  //  pinMode(EXT_LIGHT_1_PIN, OUTPUT);
  //  digitalWrite(EXT_LIGHT_1_PIN, LOW);
  //  pinMode(EXT_LIGHT_2_PIN, OUTPUT);
  //  digitalWrite(EXT_LIGHT_2_PIN, LOW);
  //  //  filter = EEPROM.read(10);
  //  pinMode(EXT_FILTER_PIN, OUTPUT);
  //  digitalWrite(EXT_FILTER_PIN, HIGH);
  //  pinMode(EXT_CO2_PIN, OUTPUT);
  //  digitalWrite(EXT_CO2_PIN, LOW);
  //  CO2 = false;
  //  pinMode(EXT_AIR_PIN, OUTPUT);
  //  digitalWrite(EXT_AIR_PIN, LOW);
  pinMode(PWM1_PIN, OUTPUT);
  digitalWrite(PWM1_PIN, LOW);
  pinMode(PWM2_PIN, OUTPUT);
  digitalWrite(PWM2_PIN, LOW);
  pinMode(PWM3_PIN, OUTPUT);
  digitalWrite(PWM3_PIN, LOW);
  //  pinMode(ALARM_PIN, OUTPUT);
  //  digitalWrite(ALARM_PIN, LOW);

  //  CoolingEnabled = EEPROM.read(5);
  LED1_start = false;
  LED2_start = false;
  LED3_start = false;
  LED1_Man_On = false;
  LED2_Man_On = false;
  LED3_Man_On = false;
  LEDMoon_start = false;

  //  PH_sonda = EEPROM.read(8);             //има ли закачена РН сонда


  max1PWM = EEPROM.read(6);
  LED1_TIME = EEPROM.read(7);
  max2PWM = EEPROM.read(62);
  LED2_TIME = EEPROM.read(72);
  max3PWM = EEPROM.read(63);
  LED3_TIME = EEPROM.read(73);
  maxMoonPWM = EEPROM.read(64);
  LEDMoon_TIME = EEPROM.read(74);
  LEDMoon_On = EEPROM.read(84);

  int cur1PWM = 0;
  int cur2PWM = 0;
  int cur3PWM = 0;

  // EEPROM_float_write(60, 7);
  // EEPROM_float_write(65, 4);
  //EEPROM_int_write(70, 1200);
  //EEPROM_int_write(75, 680);


  //  y1 = EEPROM_float_read(60); //if(y1==0) y1 = 1;
  //  y2 = EEPROM_float_read(65); //if(y2==0) y2 = 1;
  //  x1 = EEPROM_int_read(70);//if(x1==0) x1 = 1;
  //  x2 = EEPROM_int_read(75);//if(x2==0) x2 = 1;

  LCDContrast = EEPROM.read(0);
  LCDContrast = 55;
  if ((LCDContrast == 0) || (LCDContrast == 255)) {
    LCDContrast = 55;
  }

  LCDLight = EEPROM.read(1);
  myGLCD.InitLCD(LCDContrast);
  pinMode(BACK_LIGHT_PIN, OUTPUT);
  analogWrite(BACK_LIGHT_PIN, 170);

  //  int aquaT;
  //       byte at;
  //  at = EEPROM.read(2);
  //  aquaT = at + 100;
  //  minCool = EEPROM.read(3);
  //  maxCool = EEPROM.read(4);
  // Set the clock to run-mode
  rtc.halt(false);
  //    rtc.setDOW(SUNDAY);        // Set Day-of-Week to SUNDAY
  //    rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //    rtc.setDate(3, 10, 2010);   // Set the date to October 3th, 2010

  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();
  myGLCD.update();

  //-------------------------------------------------------------------------------
  //  byte b = 0;
  //  while ( (!temp.search(temp_addr)) && (b < 10)) {
  //    //Serial.println("Нет больше адресов.");
  //    temp.reset_search();
  //    delay(250);
  //    b++;
  //  }


  //  switch (temp_addr[0]) {
  //    case 0x10:
  //      //      Serial.println("  Chip = DS18S20");
  //      temp_type_s = 1;
  //      break;
  //    case 0x28:
  //      Serial.println("  Chip = DS18B20");
  //      temp_type_s = 0;
  //      break;
  //    case 0x22:
  //      Serial.println("  Chip = DS1822");
  //      temp_type_s = 0;
  //      break;
  //    default:
  //      Serial.println("Устройство не распознано");
  //  }

  currentTime = millis();
  loopTime = currentTime;
}

////-----------------------------------------------------------------------------------
//float getTemp() {
//  //  temp.reset();
//  //  temp.select(temp_addr);
//  //  temp.write(0x44, 1);        // start conversion, with parasite power on at the end
//
//  //  delay(1000);     // maybe 750ms is enough, maybe not
//  // we might do a ds.depower() here, but the reset will take care of it.
//
//  temp.reset();
//  temp.select(temp_addr);
//  temp.write(0xBE);         // Read Scratchpad
//
//  for (byte i = 0; i < 9; i++) {           // we need 9 bytes
//    temp_data[i] = temp.read();
//  }
//  //  Serial.print(OneWire::crc8(data, 8), HEX);
//
//  // Convert the data to actual temperature
//  // because the result is a 16 bit signed integer, it should
//  // be stored to an "int16_t" type, which is always 16 bits
//  // even when compiled on a 32 bit processor.
//  int16_t raw = (temp_data[1] << 8) | temp_data[0];
//
//  if (temp_type_s) {
//    raw = raw << 3; // 9 bit resolution default
//    if (temp_data[7] == 0x10) {
//      // "count remain" gives full 12 bit resolution
//      raw = (raw & 0xFFF0) + 12 - temp_data[6];
//    }
//  } else {
//    byte cfg = (temp_data[4] & 0x60);
//    // at lower res, the low bits are undefined, so let's zero them
//    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
//    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
//    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
//    //// default is 12 bit resolution, 750 ms conversion time
//  }
//  return ((float)raw / 16.0);
//}

//-----------------------------------------------------------------------------------
// Нажатые кнопки
//int button;
const byte BUTTON_NONE   = 0;
const byte BUTTON_UP     = 1;
const byte BUTTON_DOWN   = 2;
const byte BUTTON_OK     = 3;
const byte BUTTON_CANCEL = 4;

int getPressedButton()
{
  byte KeyNum = 0;
  int KeyValue1 = 0;
  int KeyValue2 = 0;

  do {
    KeyValue1 = analogRead(Key_PIN);
    delay(50);
    KeyValue2 = analogRead(Key_PIN);
    delay(50);
  } while (KeyValue1 != KeyValue2);


  if (KeyValue2 > 900)
  {
    KeyNum = BUTTON_NONE;
  }
  else if (KeyValue2 > 450)
  {
    KeyNum = BUTTON_CANCEL;
  }
  else if (KeyValue2 > 250)
  {
    KeyNum = BUTTON_DOWN;
  }
  else if (KeyValue2 > 80)
  {
    KeyNum = BUTTON_UP;
  }
  else {
    KeyNum = BUTTON_OK;
  }

  return KeyNum;
}

//-----------------------------------------------------------------------------------
void naviKey(byte maxItems) {
  if (key == BUTTON_DOWN) {
    cur_item--;
    if (cur_item < 1)
      cur_item = maxItems;
  }
  if (key == BUTTON_UP) {
    cur_item++;
    if (cur_item > maxItems)
      cur_item = 1;
  }
}
//-----------------------------------------------

void drawMenu(String mName, String item1, String item2, String item3, String item4) {
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();

  myGLCD.print(mName, 1, 0);
  //  myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), RIGHT, 0);
  myGLCD.drawLine(0, 8, 83, 8);
  myGLCD.drawLine(0, 9, 83, 9);

  if (cur_item == 1) {
    myGLCD.print(item1, 3, 12);
    myGLCD.drawRoundRect(0, 11, 83, 20);
  } else
    myGLCD.print(item1, 0, 12);

  if (cur_item == 2) {
    myGLCD.print(item2, 3, 21);
    myGLCD.drawRoundRect(0, 20, 83, 29);
  } else
    myGLCD.print(item2, 0, 21);

  if (cur_item == 3) {
    myGLCD.print(item3, 3, 30);
    myGLCD.drawRoundRect(0, 29, 83, 38);
  } else
    myGLCD.print(item3, 0, 30);

  if (cur_item == 4) {
    myGLCD.print(item4, 3, 39);
    myGLCD.drawRoundRect(0, 38, 83, 47);
  } else
    myGLCD.print(item4, 0, 39);

  myGLCD.update();
}
//-------------------------------------------------------------------------------------
byte menu_hh;
byte menu_mi;
byte menu_dd;
byte menu_mm;
int menu_yyyy;
byte menu_dow;
byte menu_off;
byte menu_item_count;
int alertAdr;
byte curLoad;
int T_ON;
int T_OFF;
int T_cur1;
int T_cur2;
int T_cur_last;
int T_cur;
byte prMin = 0;
byte prMin1 = 0;
//---------------------------------------------------------
void drawIntMenu(String mName, int minVal, int maxVal, int Val) {
  myGLCD.setFont(SmallFont);
  myGLCD.clrScr();

  myGLCD.print(mName, 1, 0);
  //  myGLCD.print(rtc.getTimeStr(FORMAT_SHORT),RIGHT,0);
  myGLCD.drawLine(0, 8, 83, 8);
  myGLCD.drawLine(0, 9, 83, 9);

  myGLCD.setFont(MediumNumbers);
  myGLCD.printNumI(Val, CENTER, 20);

  //  myGLCD.drawRoundRect(0, 38, 83, 47);

  //  int k = 80 * (Val - minVal) / (maxVal - minVal);
  //  for (int i = 2; i < k + 2; i++)
  //    myGLCD.drawLine(i, 40, i, 46);
  myGLCD.update();
}
//----------------------------------------------------------------------------------------

boolean  new_day = true;
byte secc = 0;
byte ts = 0;
byte t2s = 0;
byte t3s = 0;
byte bt;
//-----PH--------------------------------------------------------------------

//----------------------------------------
//int Temp_dr;
//int Temp;
//*****************************************************************************************
//*****************************************************************************************

void loop() {
  currentTime = millis();
  key = getPressedButton();
  if ((abs(currentTime - loopTime) >= 250) || (key != prev_key)) {

    if ((prev_key != key) && (key != 0)) {
      idle_key = 0;
    } else {
      idle_key++;
    }
    prev_key = key;

    //myGLCD.drawBitmap (0, 0, term, 30, 20);
    if (key == 0)
    {
      //      if (temp_cicle == 0) {
      //        temp.reset();
      //        temp.select(temp_addr);
      //        temp.write(0x44, 1);
    }
    //      if (temp_cicle == 4) {
    //        cur_temp = getTemp();
    //        // Управление на термостата ----------------------------------------------
    //        float T_N = (float)aquaT / 10;
    //        if (cur_temp < (float)T_N) {
    //          digitalWrite(EXT_HEATER_PIN, HIGH);   // включва нагревателя
    //        } else {
    //          if (cur_temp >= (float)T_N + 0.2)
    //            digitalWrite(EXT_HEATER_PIN, LOW);   // изключва нагревателя
    //        }
    //        if (cur_temp < ((float)T_N - 2) || cur_temp > ((float)T_N + 2))
    //          digitalWrite(ALARM_PIN, HIGH);
    //        else
    //          digitalWrite(ALARM_PIN, LOW);
    //
    //        if (CoolingEnabled) {
    //          if (cur_temp > ((float)T_N + 0.5)) {
    //            int colT_cur = 50 + (((float)(maxCool - minCool)) / 100) * ((float)(cur_temp - (T_N + 0.5))) * 60; //максимални обороти при 1,7градуса разлика
    //            //            int colT_cur =(float)(maxCool  / 100) * ((float)(cur_temp - (T_N + 0.5))) * 60;
    //            if (colT_cur > maxCool) {
    //              colT_cur = maxCool;
    //            }
    //            analogWrite(EXT_COOLING_PIN, colT_cur);   // включва вентилатора
    //          } else {
    //            if ((light1 || light2) && minCool > 20) {
    //              analogWrite(EXT_COOLING_PIN, minCool);   //включва режим проветряване
    //            } else {
    //              digitalWrite(EXT_COOLING_PIN, LOW);   //  изключва вентилатора
    //            }
    //          }
    //        }
    //      }

    //      if (temp_cicle > 51) {
    //        temp_cicle = 0;
    //      } else {
    //        temp_cicle++;
    //      }
  }

  // --------------------------------------------------------------------------------

  // Get data from the DS1307
  time = rtc.getTime();
//      Serial.print(time.hour);
//      Serial.print(F(":"));
//      Serial.print(time.min);
//      Serial.print(F(":"));
//      Serial.println(time.sec);
  //    Serial.println(EEPROM.read(520 + 4)&B00111111);
  //---------Датчик PH--------------------------------------

  //    float x;
  //    int millivolts;
  //    if (key == 0) // прави измерване само като няма натиснат бутон
  //    {
  //      x = analogRead(18);
  //
  //      for (float count = 0; count < 50; count++) {   //осреднява 50 последователни измервания
  //        x = x + constrain(analogRead(18), 0, 1023);
  //      }
  //      x = x / 50;
  //      millivolts = ((x * 5.0) / 1024) * 1000;
  //    }
  //    float kf1 = (x2 - x1) / (y1 - y2);
  //    float kf2 = kf1 * y1 + x1;
  //    float pH_new = (kf2 - millivolts) / kf1;
  //
  //    int pH_d = (pH_new - (int)pH_new) * 100;   //Отделяне на дробната част от PH

  //---------------------------------------------------------------------------------
  // Стартира диодите
  //byte prSec;
  if (prSec != time.sec) {
    prSec = time.sec;

    ts ++;
    //      if (LED1_start) {
    //      Serial.println("LED1_start");
    //    }else {
    //      Serial.println("LED1_stop");
    //    }
    //       if (LED2_start) {
    //      Serial.println("LED2_start");
    //    }else {
    //      Serial.println("LED2_stop");
    //    }
    //    if (LED3_start) {
    //      Serial.println("LED3_start");
    //    }else {
    //      Serial.println("LED3_stop");
    //    }

    if (ts >= 39) {  //отговаря на 10секунди
      ts = 0;
      if (LED1_start) {
        if (LEDMoon_On && ((time.hour > 22) || (time.hour < 6))) {
          max1PWM = maxMoonPWM;
          LED1_TIME = LEDMoon_TIME;
        } else {
          max1PWM = max1PWM;
          LED1_TIME = LED1_TIME;
        }
        if (LED1_Man_On == false) {
          cur1PWM = (float)((float)cur1PWM) + ((float)max1PWM / LED1_TIME) / 6;

          if (cur1PWM > max1PWM)
          {
            cur1PWM = max1PWM;
          }
          analogWrite(PWM1_PIN, cur1PWM);
        }
      } else {
        if (!(LED1_Man_On)) {
          cur1PWM = (float)((float)cur1PWM) - ((float)max1PWM / LED1_TIME) / 6;
          analogWrite(PWM1_PIN, cur1PWM);
          if (cur1PWM < 0)
          {
            cur1PWM = 0;
          }
          analogWrite(PWM1_PIN, cur1PWM);
        }
      }

      if (LED2_start) {
        if (LEDMoon_On && ((time.hour > 22) || (time.hour < 6))) {
          max2PWM = maxMoonPWM;
          LED2_TIME = LEDMoon_TIME;
          LED2_TIME = LED2_TIME;
        } else {
          max2PWM = max2PWM;
        }
        if (LED2_Man_On == false) {
          cur2PWM = (float)((float)cur2PWM) + ((float)max2PWM / LED2_TIME) / 6;

          if (cur2PWM > max2PWM)
          {
            cur2PWM = max2PWM;
          }
          analogWrite(PWM2_PIN, cur2PWM);
        }
      } else {
        if (!(LED2_Man_On)) {
          cur2PWM = (float)((float)cur2PWM) - ((float)max2PWM / LED2_TIME) / 6;
          analogWrite(PWM2_PIN, cur2PWM);
          if (cur2PWM < 0)
          {
            cur2PWM = 0;
          }
          analogWrite(PWM2_PIN, cur2PWM);
        }
      }
      if (LED3_start) {
        if (LED3_Man_On == false) {
          cur3PWM = (float)((float)cur3PWM) + ((float)max3PWM / LED3_TIME) / 6;

          if (cur3PWM > max3PWM)
          {
            cur3PWM = max3PWM;
          }
          analogWrite(PWM3_PIN, cur3PWM);
        }
      } else {
        if (!(LED3_Man_On)) {
          cur3PWM = (float)((float)cur3PWM) - ((float)max3PWM / LED3_TIME) / 6;
          analogWrite(PWM3_PIN, cur3PWM);
          if (cur3PWM < 0)
          {
            cur3PWM = 0;
          }
          analogWrite(PWM3_PIN, cur3PWM);
        }
      }
    }
  }
  //------------------------------------------------------------------------------------

  // Нова минута ----------------------------------------------
  if (prMin != time.min) {
    prMin = time.min;
    //-----------------------------------------------
    int a;
    for (a = 100; a < 400; a = a + 10) {
      byte b1 = EEPROM.read(a);
//      Serial.print(F(" A: "));
//      Serial.print(b1 & B10000000);
//      Serial.print(F(" A+ 1: "));
//      Serial.print(a + 1);
//      Serial.print(F(" T_ON: "));
//      Serial.print(EEPROM.read(a + 1));
//      Serial.print(F(":"));
//      Serial.print(EEPROM.read(a + 2));
//      Serial.print(F(" T_OFF: "));
//      Serial.print(EEPROM.read(a + 3));
//      Serial.print(F(":"));
//      Serial.print(EEPROM.read(a + 4));
//      Serial.print(F(" Da?"));
//      Serial.print(EEPROM.read(a)&B10000000);
//      Serial.print(F(" Den?:"));
//      Serial.print(EEPROM.read(a)&B00000001);
//      Serial.print(F(" a & B00000001: "));
//      Serial.print(a & B00000001);
//      Serial.print(F(" a & B10000000: "));
//      Serial.print(a & B10000000);
//      Serial.print(F(" B00011111: "));
//      Serial.print(B00011111);
//      Serial.print(F(" a & B00011111: "));
//      Serial.print(a & B00011111);
//      Serial.print(F(" curLoad: "));
//      Serial.println(EEPROM.read(a + 5));

      T_ON = EEPROM.read(a + 1) * 60 + EEPROM.read(a + 2);
      T_OFF = EEPROM.read(a + 3) * 60 + EEPROM.read(a + 4);
      T_cur1 = time.hour * 60 + time.min;
      delay(50);
      time = rtc.getTime();
      T_cur2 = time.hour * 60 + time.min;;
      if (T_cur1 == T_cur2 ) {
        T_cur = T_cur1;
      } else {
        T_cur = T_cur_last + 1;
      }
      T_cur_last = T_cur;
      
      if (b1 & B10000000) {
        if ((1 << (time.dow - 1))&b1) {          // разрешен ли е деня от седмицата
          boolean tajmer = false;
          if (T_ON < T_OFF) //Проверка на таймера в едно денонощие
          {
            if (T_cur >= T_ON && T_cur <= T_OFF)
            {
              tajmer = true;
            }    else      {
              tajmer = false;
            }
          } else { // ако се е сменил деня
            if ((T_cur >= T_ON && T_cur <= 2820) || (T_cur >= 0 && T_cur <= T_OFF)) //2820=24часа*60
            {
              tajmer = true;
            }      else {
              tajmer = false;
            }
          }

//          Serial.println(LED1_start);
          if (tajmer) {
//             Serial.print(F("T_ON:"));
//             Serial.print(T_ON);
//             Serial.print(F("T_OFF:"));
//             Serial.print(T_OFF);
//             Serial.print(F("T_cur:"));
//             Serial.println(T_cur);
            byte b2 = EEPROM.read(a + 5);
//            Serial.print(F(" B2: "));
//            Serial.println(b2);
            switch (b2) {                              // определяме таймера
              case 1:                                   //осветление 1
              Serial.println(T_cur);
                LED1_start = true;
                if ((T_cur >= T_OFF) && (!(LEDMoon_On && ((time.hour > 22) || (time.hour < 6)))))  {
                  LED1_start = false;
                }
                break;
              case 2:                              //осветление 2
                LED2_start = true;
                if ((T_cur >= T_OFF) && (!(LEDMoon_On && ((time.hour > 22) || (time.hour < 6))))) {
                  LED2_start = false;
                }
                break;
              case 3:
                LED3_start = true;
                if (T_cur >= T_OFF){
                  LED3_start = false;
                }
                break;
            }
          }
        }
      }
    }
  }
  //----------------Управление на осветление ------------------------------------------
  //    if ((light1 || light11) && light12) {
  //      digitalWrite(EXT_LIGHT_1_PIN, HIGH);
  //      light1 = true;
  //    } else {
  //      digitalWrite(EXT_LIGHT_1_PIN, LOW);
  //      light1 = false;
  //    }
  //------------------------------------------------------------------------------------
  //    if ((light2 || light21) && light22) {
  //      digitalWrite(EXT_LIGHT_2_PIN, HIGH);
  //      light2 = true;
  //    } else {
  //      digitalWrite(EXT_LIGHT_2_PIN, LOW);
  //      light2 = false;
  //    }
  //-----------------Управление СО2---------------------------------------------------
  //    if ((CO2 || CO2_1) && CO2_2) {
  //      digitalWrite(EXT_CO2_PIN, HIGH);
  //      CO2 = true;
  //    } else {
  //      digitalWrite(EXT_CO2_PIN, LOW);
  //      CO2 = false;
  //    }
  //----------------Управление аератор------------------------------------------------
  //    if ( AIR ) {
  //      digitalWrite(EXT_AIR_PIN, HIGH);
  //    } else {
  //      digitalWrite(EXT_AIR_PIN, LOW);
  //    }
  //------------------------------------------------------------------------------------
  if ((idle_key > max_idle_key) && (cur_menu != 0) && (cur_menu != 135)) {
    cur_menu = 0;
    cur_item = 0;
    cur_item_edit = false;
  }

  switch (cur_menu) {
    case 0: // Time
      {
        analogWrite(BACK_LIGHT_PIN, 220);      //подава на изхода мин.подсветка на екрана
        //          myGLCD.setFont(MediumNumbers);
        myGLCD.clrScr();
        myGLCD.setFont(SmallFont);
        myGLCD.print(rtc.getTimeStr(), RIGHT, 0);

        //          if (cur_temp < 1) {
        //myGLCD.print("temperatura na", CENTER, 15);
        //myGLCD.print("swetlinata", CENTER, 30);
        //            byte b = 0;
        //            while ( (!temp.search(temp_addr)) && (b < 2)) {
        //              //Serial.println("Нет больше адресов.");
        //              temp.reset_search();
        //              ;
        //              b++;
        //            }
        //          }
        //          else {
        myGLCD.setFont(SmallFont);
        myGLCD.print("beli", 30, 12);
        myGLCD.drawCircle(60, 14, 3);
        if (LED1_start || LED1_Man_On) {
          myGLCD.drawCircle(60, 14, 2);
          myGLCD.drawCircle(60, 14, 1);
        }
        myGLCD.print("sini", 30, 22);
        myGLCD.drawCircle(60, 24, 3);
        if (LED2_start || LED2_Man_On) {
          myGLCD.drawCircle(60, 24 , 2);
          myGLCD.drawCircle(60, 24, 1);
        }
        myGLCD.print("`erweni", 10, 32);
        myGLCD.drawCircle(60, 34, 3);
        if (LED3_start || LED3_Man_On) {
          myGLCD.drawCircle(60, 34 , 2);
          myGLCD.drawCircle(60, 34, 1);
        }
        //            myGLCD.setFont(MediumNumbers);
        //            myGLCD.print("/", 72, 10);
        //            int temp1 = (cur_temp - (int)cur_temp) * 100;   //Отделяне на дробната част от температурата
        //            myGLCD.printNumI(cur_temp, 6, 10, 2, '0');
        //            myGLCD.printNumI(temp1, 33, 10, 2, '0');
        //myGLCD.drawRect(31, 24, 32, 25);
        //          }
        //       if (PH_sonda) {
        //myGLCD.setFont(MediumNumbers);
        //            if ( pH_new < 1 || pH_new > 13) {
        //myGLCD.setFont(SmallFont);
        //myGLCD.print("6450 K", CENTER, 40);
        //
        //            } else {
        //              int pH_d = (pH_new - (int)pH_new) * 100;   //Отделяне на дробната част от pH
        //              myGLCD.printNumI(pH_new, 6, 30, 2, '.');
        //              myGLCD.printNumI(pH_d, 33, 30, 2, '0');
        //              myGLCD.drawRect(31, 44, 32, 45);
        //              myGLCD.setFont(SmallFont);
        //              myGLCD.print ("PH" , 65, 32);
        //            }
        //         }
        myGLCD.update();
        if (key == 3)
          cur_menu = 13;
        cur_item = 0;
        cur_item_edit = false;

        if (key == 1) {
          LED1_Man_On = ! LED1_Man_On;
          if (LED1_Man_On) {
            analogWrite(PWM1_PIN, 255); // включва червените
          } else {
            analogWrite(PWM1_PIN, 0); // изключва червените
            LED1_Man_On = false;
          }
        }
        if (key == 2) {
          LED2_Man_On = ! LED2_Man_On;
          if (LED2_Man_On) {
            analogWrite(PWM2_PIN, 255); // включва червените
          } else {
            analogWrite(PWM2_PIN, 0); // изключва червените
            LED2_Man_On = false;
          }
        }
        if (key == 4) {
          LED3_Man_On = ! LED3_Man_On;
          if (LED3_Man_On) {
            analogWrite(PWM3_PIN, 255); // включва червените
          } else {
            analogWrite(PWM3_PIN, 0); // изключва червените
            LED3_Man_On = false;
          }
        }

        break;
      }


    case 13: //---------------------------------------------------------------
      {
        analogWrite(BACK_LIGHT_PIN, 170);  //LCDLight

        drawMenu("men/ ", /*"termostat",*/ "oswetlenie", /*"CO2-aerator",*/ "sistemni", " ", " ");
        naviKey(2);
        switch (key) {
          case BUTTON_OK:
            switch (cur_item) {
              //                case 1:
              //                //Asen
              //                  cur_menu = 132;
              //                  cur_item = 0;
              //                  break;
              case 1:
                cur_menu = 130;
                cur_item = 1;
                break;
              //                case 2:
              //                  cur_menu = 134;
              //                  cur_item = 0;
              //                  break;
              case 2:
                cur_menu = 131;
                cur_item = 0;
                break;
            }
            break;
          case BUTTON_CANCEL:
            cur_menu = 0;
            cur_item = 0;
            break;
        }
        break;
      }
    //      case 134: // UDO -----------------------------------------------------------------
    //        {
    //          String uu;
    //          if (CO2) {
    //            uu = "ON";
    //          } else {
    //            uu = "OFF";
    //          }
    //          //          drawMenu("CO2 aerator", "CO2", "CO2 " + uu, "aerator", "kalibrowka rn ");
    //          if (PH_sonda) {
    //            naviKey(4);
    //            drawMenu("CO2 aerator", "CO2", "CO2 " + uu, "aerator", "kalibriwka rn ");
    //          } else {
    //            naviKey(3);
    //            drawMenu("CO2 aerator", "CO2", "CO2 " + uu, "aerator", "");
    //          }
    //          switch (key) {
    //            case BUTTON_OK:
    //              switch (cur_item) {
    //                case 1:
    //                  cur_menu = 133;
    //                  cur_item = 1;
    //                  menu_off = 1;
    //                  menu_item_count = 4;
    //                  curLoad = 4;
    //                  break;
    //
    //                case 2:
    //                                  cur_menu = 140;
    //                  CO2 = !CO2;
    //                  if (CO2) {
    //                    CO2_2 = true;
    //                    //                    digitalWrite(EXT_CO2_PIN, HIGH);
    //                  } else {
    //                    CO2_2 = false;
    //                    //                    digitalWrite(EXT_CO2_PIN, LOW);
    //                  }
    //                  break;
    //
    //                case 3:
    //                  cur_menu = 133;
    //                  cur_item = 1;
    //                  menu_off = 1;
    //                  menu_item_count = 4;
    //                  curLoad = 3;
    //                  break;
    //
    //                case 4:
    //                  cur_menu = 135;
    //                  cur_item = 1;
    //                  cur_item_edit = false;
    //
    //                  break;
    //              }
    //              break;
    //            case BUTTON_CANCEL:
    //              cur_menu = 13;
    //              cur_item = 3;
    //              break;
    //          }
    //          break;
    //        }
    //-----------------------------------------------------
    //case 140:
    //{
    //          myGLCD.setFont(SmallFont);
    //          myGLCD.clrScr();
    //
    //          myGLCD.print("temperatura", 1, 0);
    //          //  myGLCD.print(rtc.getTimeStr(FORMAT_SHORT),RIGHT,0);
    //          myGLCD.drawLine(0, 8, 83, 8);
    //          myGLCD.drawLine(0, 9, 83, 9);
    //          myGLCD.setFont(MediumNumbers);
    //          Temp_dr = aquaT % 10;
    //          Temp  = aquaT / 10;
    //          myGLCD.printNumI(Temp, 14, 15, 2);
    //          myGLCD.printNumI(Temp_dr, 41, 15, 1);
    //          myGLCD.drawRect(39, 29, 40, 30);
    //          //  myGLCD.drawRoundRect(0, 38, 83, 47);
    //          //  int k = 80 * (aquaT - 180) / (350 - 180);
    //          //  for (int i = 2; i < k + 2; i++)
    //          //    myGLCD.drawLine(i, 40, i, 46);
    //          myGLCD.update();
    //
    //          // at = aquaT-100;
    //
    //          //          drawIntMenu("temperatura", 18, 35, aquaT); //Aqua Temperat
    //          switch (key) {
    //            case BUTTON_UP:
    //              aquaT++;
    //              aquaT = min(aquaT, 350);
    //              break;
    //            case BUTTON_DOWN:
    //              aquaT--;
    //              aquaT = max(aquaT, 180);
    //              break;
    //            case BUTTON_OK:
    //              at = aquaT - 100;
    //              EEPROM.write(2, at);
    //              cur_menu = 132;
    //              cur_item = 1;
    //              break;
    //            case BUTTON_CANCEL:
    //              //              aquaT = EEPROM.read(2) + 100;
    //              cur_item = 1;
    //              break;
    //          }
    //break;
    //}
    //--------------------------------------------------------
    case 130: //----------------------------------------------------------------
      {
        drawMenu("oswetlenie", "kanal beli", "kanal sini", "kanal `erweni", "izgrew&zalez");
        naviKey(4);
        switch (key) {
          case BUTTON_OK:
            switch (cur_item) {
              case 1:
                cur_menu = 133;
                cur_item = 1;
                menu_off = 1;
                menu_item_count = 4;
                curLoad = 1;
                break;
              case 2:
                cur_menu = 133;
                cur_item = 1;
                menu_off = 1;
                menu_item_count = 4;
                curLoad = 2;
                break;
              case 3:
                cur_menu = 133;
                cur_item = 1;
                menu_off = 1;
                menu_item_count = 4;
                curLoad = 3;
                break;
              case 4:
                cur_menu = 1400;
                cur_item = 1;
                cur_item_edit = false;
                break;
            }
            break;
          case BUTTON_CANCEL:
            cur_menu = 13;
            cur_item = 1;
            break;
        }
        break;
      }
    //--------------------------------------------------------
    case 1400: //----------------------------------------------------------------
      {
        drawMenu("izgrew&zalez", "kanal beli", "kanal sini", "kanal `erweni", "luna");
        naviKey(4);
        switch (key) {
          case BUTTON_OK:
            switch (cur_item) {
              case 1:
                cur_menu = 1500;
                cur_item = 1;
                break;
              case 2:
                cur_menu = 1600;
                cur_item = 1;
                break;
              case 3:
                cur_menu = 1700;
                cur_item = 1;
                break;
              case 4:
                cur_menu = 1800;
                cur_item = 1;
                break;
            }
            break;
          case BUTTON_CANCEL:
            cur_menu = 130;
            cur_item = 4;
            break;
        }
        break;
      }
    //--------------------------------------------------------
    //      case 121: //----------------------------------------------------------------
    //        {
    //          drawMenu("oswetlenie", "oswetlenie 1", "oswetlenie 2", "Kanal sini", "LED nastrojka");
    //          naviKey(4);
    //          switch (key) {
    //            case BUTTON_OK:
    //              switch (cur_item) {
    //                case 1:
    //                  cur_menu = 133;
    //                  cur_item = 1;
    //                  menu_off = 1;
    //                  menu_item_count = 3;
    //                  curLoad = 1;
    //                  break;
    //                case 2:
    //                  cur_menu = 133;
    //                  cur_item = 1;
    //                  menu_off = 1;
    //                  menu_item_count = 4;
    //                  curLoad = 2;
    //                  break;
    //                case 3:
    //                  cur_menu = 133;
    //                  cur_item = 1;
    //                  menu_off = 1;
    //                  menu_item_count = 4;
    //                  curLoad = 5;
    //                  break;
    //                case 4:
    //                  cur_menu = 1500;
    //                  cur_item = 1;
    //                  cur_item_edit = false;
    //                  break;
    //              }
    //              break;
    //            case BUTTON_CANCEL:
    //              cur_menu = 13;
    //              cur_item = 2;
    //              break;
    //          }
    //          break;
    //        }
    //------------------------------------------------------------------------------
    case 1500: // LED1PWM -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();
        myGLCD.print("mo]tnost", 1, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        //          myGLCD.drawLine(0, 9, 83, 9);
        myGLCD.printNumI(max1PWM, CENTER, 19);
        myGLCD.printNumI(LED1_TIME, CENTER, 38);
        //          analogWrite(PWM1_PIN, max1PWM);
        if ((cur_item == 1) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("maksimum", CENTER, 11);
        myGLCD.invertText(false);

        if ((cur_item == 2) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("period", CENTER, 30);
        myGLCD.invertText(false);

        if (cur_item == 1) {
          //    myGLCD.print("k1", 3, 12);
          myGLCD.drawRoundRect(0, 10, 83, 28);
        }
        if (cur_item == 2) {
          //    myGLCD.print("k2", 3, 21);
          myGLCD.drawRoundRect(0, 28, 83, 47);
        }
        switch (key) {
          case BUTTON_UP:
            if (!cur_item_edit) {
              cur_item--;
              if (cur_item < 1)
                cur_item = 2;
            } else {
              if (cur_item == 1) {
                max1PWM++;
                if (max1PWM == 255)
                  max1PWM = 0;
              }
              if (cur_item == 2) {
                LED1_TIME++;
                if (LED1_TIME == 120)
                  LED1_TIME = 0;
              }
            }
            break;
          case BUTTON_DOWN:
            if (!cur_item_edit) {
              cur_item++;
              if (cur_item > 2)
                cur_item = 1;
            } else {
              if (cur_item == 1) {
                max1PWM--;
                if (max1PWM == 0)
                  max1PWM = 255;
              }
              if (cur_item == 2) {
                LED1_TIME--;
                if (LED1_TIME == 0)
                  LED1_TIME = 120;
              }

            }
            break;
          case BUTTON_OK:
            if (cur_item_edit) {
              switch (cur_item) {
                case 1:
                  EEPROM.write (6, max1PWM); //запис на у1 в EEPROM
                  max1PWM = EEPROM.read(6);
                  break;
                case 2:

                  EEPROM.write (7, LED1_TIME); //запис на у1 в EEPROM
                  LED1_TIME = EEPROM.read(7);
                  break;
              }
              cur_item_edit = false;
            } else
              cur_item_edit = true;
            break;

          case BUTTON_CANCEL:
            cur_item_edit = false;
            cur_menu = 1400;
            cur_item = 1;
            break;
        }

        myGLCD.update();

        break;
      }
    case 1600: // LED2PWM -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();
        myGLCD.print("izgrew&zalez", 1, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        //          myGLCD.drawLine(0, 9, 83, 9);
        myGLCD.printNumI(max2PWM, CENTER, 19);
        myGLCD.printNumI(LED2_TIME, CENTER, 38);
        //          analogWrite(PWM1_PIN, max1PWM);
        if ((cur_item == 1) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("maksimum", CENTER, 11);
        myGLCD.invertText(false);

        if ((cur_item == 2) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("period", CENTER, 30);
        myGLCD.invertText(false);

        if (cur_item == 1) {
          //    myGLCD.print("k1", 3, 12);
          myGLCD.drawRoundRect(0, 10, 83, 28);
        }
        if (cur_item == 2) {
          //    myGLCD.print("k2", 3, 21);
          myGLCD.drawRoundRect(0, 28, 83, 47);
        }
        switch (key) {
          case BUTTON_UP:
            if (!cur_item_edit) {
              cur_item--;
              if (cur_item < 1)
                cur_item = 2;
            } else {
              if (cur_item == 1) {
                max2PWM++;
                if (max2PWM == 255)
                  max2PWM = 0;
              }
              if (cur_item == 2) {
                LED2_TIME++;
                if (LED2_TIME == 120)
                  LED2_TIME = 0;
              }
            }
            break;
          case BUTTON_DOWN:
            if (!cur_item_edit) {
              cur_item++;
              if (cur_item > 2)
                cur_item = 1;
            } else {
              if (cur_item == 1) {
                max2PWM--;
                if (max2PWM == 0)
                  max2PWM = 255;
              }
              if (cur_item == 2) {
                LED2_TIME--;
                if (LED2_TIME == 0)
                  LED2_TIME = 120;
              }

            }
            break;
          case BUTTON_OK:
            if (cur_item_edit) {
              switch (cur_item) {
                case 1:
                  EEPROM.write (62, max2PWM); //запис на у1 в EEPROM
                  max2PWM = EEPROM.read(62);
                  break;
                case 2:

                  EEPROM.write (72, LED2_TIME); //запис на у1 в EEPROM
                  LED2_TIME = EEPROM.read(72);
                  break;
              }
              cur_item_edit = false;
            } else
              cur_item_edit = true;
            break;

          case BUTTON_CANCEL:
            cur_item_edit = false;
            cur_menu = 1400;
            cur_item = 2;
            break;
        }

        myGLCD.update();

        break;
      }
    case 1700: // LED3PWM -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();
        myGLCD.print("izgrew&zalez", 1, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        //          myGLCD.drawLine(0, 9, 83, 9);
        myGLCD.printNumI(max3PWM, CENTER, 19);
        myGLCD.printNumI(LED3_TIME, CENTER, 38);
        //          analogWrite(PWM1_PIN, max1PWM);
        if ((cur_item == 1) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("maksimum", CENTER, 11);
        myGLCD.invertText(false);

        if ((cur_item == 2) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("period", CENTER, 30);
        myGLCD.invertText(false);

        if (cur_item == 1) {
          //    myGLCD.print("k1", 3, 12);
          myGLCD.drawRoundRect(0, 10, 83, 28);
        }
        if (cur_item == 2) {
          //    myGLCD.print("k2", 3, 21);
          myGLCD.drawRoundRect(0, 28, 83, 47);
        }
        switch (key) {
          case BUTTON_UP:
            if (!cur_item_edit) {
              cur_item--;
              if (cur_item < 1)
                cur_item = 2;
            } else {
              if (cur_item == 1) {
                max3PWM++;
                if (max3PWM == 255)
                  max3PWM = 0;
              }
              if (cur_item == 2) {
                LED3_TIME++;
                if (LED3_TIME == 120)
                  LED3_TIME = 0;
              }
            }
            break;
          case BUTTON_DOWN:
            if (!cur_item_edit) {
              cur_item++;
              if (cur_item > 2)
                cur_item = 1;
            } else {
              if (cur_item == 1) {
                max3PWM--;
                if (max3PWM == 0)
                  max3PWM = 255;
              }
              if (cur_item == 2) {
                LED3_TIME--;
                if (LED3_TIME == 0)
                  LED3_TIME = 120;
              }

            }
            break;
          case BUTTON_OK:
            if (cur_item_edit) {
              switch (cur_item) {
                case 1:
                  EEPROM.write (63, max3PWM); //запис на у1 в EEPROM
                  max3PWM = EEPROM.read(63);
                  break;
                case 2:

                  EEPROM.write (73, LED3_TIME); //запис на у1 в EEPROM
                  LED3_TIME = EEPROM.read(73);
                  break;
              }
              cur_item_edit = false;
            } else
              cur_item_edit = true;
            break;

          case BUTTON_CANCEL:
            cur_item_edit = false;
            cur_menu = 1400;
            cur_item = 3;
            break;
        }

        myGLCD.update();

        break;
      }
    case 1800: // LED3PWM -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();
        myGLCD.print("luna", 1, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        //          myGLCD.drawLine(0, 9, 83, 9);
        myGLCD.printNumI(maxMoonPWM, 65, 11);
        myGLCD.printNumI(LEDMoon_TIME, 65, 21);
        //          analogWrite(PWM1_PIN, max1PWM);
        if ((cur_item == 1) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("maksimum", 3, 11);
        myGLCD.invertText(false);

        if ((cur_item == 2) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.print("period", 3, 21);
        myGLCD.invertText(false);

        if ((cur_item == 3) && cur_item_edit)
          myGLCD.invertText(true);
        if (LEDMoon_On) {
          allow = "pozwoleno";
        } else {
          allow = "zabraneno";
        }

        myGLCD.print(allow, CENTER, 31);
        myGLCD.invertText(false);

        if (cur_item == 1) {
          //    myGLCD.print("k1", 3, 12);
          myGLCD.drawRoundRect(0, 9, 83, 19);
        }
        if (cur_item == 2) {
          //    myGLCD.print("k2", 3, 21);
          myGLCD.drawRoundRect(0, 19 , 83, 29);
        }
        if (cur_item == 3) {
          //    myGLCD.print("k2", 3, 21);
          myGLCD.drawRoundRect(0, 29, 83, 39);
        }
        switch (key) {
          case BUTTON_UP:
            if (!cur_item_edit) {
              cur_item++;
              if (cur_item > 3)
                cur_item = 3;
            } else {
              if (cur_item == 1) {
                maxMoonPWM++;
                if (maxMoonPWM == 75)
                  maxMoonPWM = 0;
              }
              if (cur_item == 2) {
                LEDMoon_TIME++;
                if (LEDMoon_TIME == 120)
                  LEDMoon_TIME = 0;
              }
              if (cur_item == 3) {
                LEDMoon_On = ! LEDMoon_On;
              }
            }
            break;
          case BUTTON_DOWN:
            if (!cur_item_edit) {
              cur_item--;
              if (cur_item < 1)
                cur_item = 3;
            } else {
              if (cur_item == 1) {
                maxMoonPWM--;
                if (maxMoonPWM == 0)
                  maxMoonPWM = 75;
              }
              if (cur_item == 2) {
                LEDMoon_TIME--;
                if (LEDMoon_TIME == 0)
                  LEDMoon_TIME = 120;
              }
              if (cur_item == 3) {
                LEDMoon_On = ! LEDMoon_On;
              }

            }
            break;
          case BUTTON_OK:
            if (cur_item_edit) {
              switch (cur_item) {
                case 1:
                  EEPROM.write (64, maxMoonPWM); //запис на у1 в EEPROM
                  maxMoonPWM = EEPROM.read(64);
                  break;
                case 2:

                  EEPROM.write (74, LEDMoon_TIME); //запис на у1 в EEPROM
                  LEDMoon_TIME = EEPROM.read(74);
                  break;
                case 3:

                  EEPROM.write (84, LEDMoon_On); //запис на у1 в EEPROM
                  LEDMoon_On = EEPROM.read(84);
                  break;
              }
              cur_item_edit = false;
            } else
              cur_item_edit = true;
            break;

          case BUTTON_CANCEL:
            cur_item_edit = false;
            cur_menu = 1400;
            cur_item = 4;
            break;
        }

        myGLCD.update();

        break;
      }
    //----------------------------------------------------------------------------
    case 133: // -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();
        int offf;
        switch (curLoad) {
          case 1:
            myGLCD.print("beli", 1, 0); //Beli//Light 1
            offf = 100;
            break;
          case 2:
            myGLCD.print("sini", 1, 0); //Light 2
            offf = 200;
            break;
          case 3:
            myGLCD.print("`erweni", 1, 0); //Air
            offf = 300;
            break;
            //            case 4:
            //              myGLCD.print("CO2", 1, 0); //CO2
            //              offf = 500;
            //              break;
            //            case 5:
            //              myGLCD.print("sini", 1, 0);
            //              offf = 400;
            //              break;

        }

        myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), RIGHT, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        myGLCD.drawLine(0, 9, 83, 9);

        String tt;
        byte vy;
        for (byte j = menu_off; j <= min(menu_off + 4, menu_item_count); j++) {
          vy = 3 + 9 * (j - menu_off + 1);

          alertAdr = offf + (j - 1) * 10;
          bt = EEPROM.read(alertAdr + 1)&B00011111;
          tt = (String)(j) + " ";
          if (bt < 10) {
            tt = tt + "0";
          }
          tt = tt + (String)bt + ":";

          bt = EEPROM.read(alertAdr + 2)&B00111111;
          if (bt < 10) {
            tt = tt + "0";
          }
          tt = tt + (String)bt + "-";
          //-----------------------------------------------------------------------------
          bt = EEPROM.read(alertAdr + 3)&B00011111;
          //        tt = (String)(j) + " ";
          if (bt < 10) {
            tt = tt + "0";
          }
          tt = tt + (String)bt + ":";

          bt = EEPROM.read(alertAdr + 4)&B00111111;
          if (bt < 10) {
            tt = tt + "0";
          }
          tt = tt + (String)bt;

          if (cur_item == j) {
            myGLCD.print(tt, 3, vy);
            myGLCD.drawRoundRect(0, vy - 1, 83, vy + 8);
          } else
            myGLCD.print(tt, 0, vy);

          // слага черта на не активните таймери
          if (!(EEPROM.read(offf + (j - 1) * 10)&B10000000)) {
            myGLCD.drawLine(0, vy + 5, 83, vy + 5);
          }

        }

        switch (key) {
          case BUTTON_DOWN:
            if (cur_item == 1) {
              cur_item = menu_item_count;
              menu_off = menu_item_count - 3;
            } else {
              cur_item--;
              if (cur_item < menu_off) {
                menu_off = cur_item;
              }
            }
            break;
          case BUTTON_UP:
            if (cur_item == menu_item_count) {
              cur_item = 1;
              menu_off = 1;
            } else {
              cur_item++;
              if (cur_item > menu_off + 3) {
                menu_off = cur_item - 3;
              }
            }
            break;
          case BUTTON_OK:
            alertAdr = offf + (cur_item - 1) * 10;
            cur_menu = 1330;
            cur_item = 1;
            cur_item_edit = false;
            break;
          case BUTTON_CANCEL:
            switch (curLoad) {
              case 1:
                //"Beli",
                cur_menu = 130;
                cur_item = 1;
                break;
              case 2:
                //"sini"
                cur_menu = 130;
                cur_item = 2;
                break;
              case 3:
                //"`erweni"
                cur_menu = 130;
                cur_item = 3;
                break;
                //                case 4:
                //                  //"CO2"
                //                  cur_menu = 134;
                //                  cur_item = 1;
                //                  break;
                //                case 5:
                //                  //"LED"
                //                  cur_menu = 130;
                //                  cur_item = 3;
                //                  break;

            }

            cur_item_edit = false;
            break;
        }
        myGLCD.update();
        break;
      }
    case 1330: //  меню на таймерите-------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();

        switch (curLoad) {
          case 1:
            myGLCD.print("beli", 1, 0);//beli //Light1
            break;
          case 2:
            myGLCD.print("sini", 1, 0); //Sini
            break;
          case 3:
            myGLCD.print("`erweni", 1, 0); //cherveni
            break;
            //            case 4:
            //              myGLCD.print("CO2", 1, 0);
            //              break;
            //            case 5:
            //              myGLCD.print("LED", 1, 0);
            //              break;

        }

        myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), RIGHT, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        myGLCD.drawLine(0, 9, 83, 9);
        byte b1 = EEPROM.read(alertAdr);
        bt = b1;
        myGLCD.print("p w s ` p s n", 0, 11);
        for (int j = 1; j < 8; j++) {

          if (cur_item == j) {
            myGLCD.drawRect(j * 12 - 10, 20, j * 12 - 2, 28);
          }
          myGLCD.drawCircle(j * 12 - 6, 24, 3);
          if (bt & B00000001) {
            myGLCD.drawCircle(j * 12 - 6, 24, 2);
            myGLCD.drawCircle(j * 12 - 6, 24, 1);
          }
          bt = bt >> 1;
        }
        String stat = "";
        if (cur_item == 8) {
          stat = stat + "[";
        }
        if (EEPROM.read(alertAdr)&B10000000) {
          stat = stat + "da";
        } else
          stat = stat + "ne";
        if (cur_item == 8) {
          stat = stat + "]";
        }

        myGLCD.print(stat, 60, 29);

        stat = "ON:";
        if (cur_item == 9) {
          stat = stat + "[";
        }
        bt = EEPROM.read(alertAdr + 1)&B00011111;
        if (bt < 10) {
          stat = stat + "0";
        }
        stat = stat + (String)bt;
        if (cur_item == 9) {
          stat = stat + "]";
        }

        stat = stat + ":";

        if (cur_item == 10) {
          stat = stat + "[";
        }
        bt = EEPROM.read(alertAdr + 2)&B00111111;
        if (bt < 10) {
          stat = stat + "0";
        }
        stat = stat + (String)bt;
        if (cur_item == 10) {
          stat = stat + "]";
        }
        //------------------------------------------------

        myGLCD.print(stat, 0, 29);
        //-----------------------------------------------------------------------------------------
        stat = "OFF:";
        if (cur_item == 11) {
          stat = stat + "[";
        }
        bt = EEPROM.read(alertAdr + 3)&B00011111;
        if (bt < 10) {
          stat = stat + "0";
        }
        stat = stat + (String)bt;
        if (cur_item == 11) {
          stat = stat + "]";
        }

        stat = stat + ":";

        if (cur_item == 12) {
          stat = stat + "[";
        }
        bt = EEPROM.read(alertAdr + 4)&B00111111;
        if (bt < 10) {
          stat = stat + "0";
        }
        stat = stat + (String)bt;
        if (cur_item == 12) {
          stat = stat + "]";
        }

        myGLCD.print(stat, 0, 39);
        //----------------------------------------------------
        if ((cur_item == 9) && cur_item_edit)
          myGLCD.drawLine(20, 36, 41, 36);
        if ((cur_item == 10) && cur_item_edit)
          myGLCD.drawLine(38, 36, 59, 36);
        if ((cur_item == 11) && cur_item_edit)
          myGLCD.drawLine(26, 46, 47, 46);
        if ((cur_item == 12) && cur_item_edit)
          myGLCD.drawLine(44, 46, 65, 46);
        //----------------------------------------------------

        switch (key) {
          case BUTTON_UP:
            if (cur_item_edit) {
              if (cur_item == 9) {
                bt = (EEPROM.read(alertAdr + 1)&B00011111);
                if (bt == 23) {
                  bt = 0;
                } else
                  bt++;
                EEPROM.write(alertAdr + 1, bt);
              }
              if (cur_item == 10) {
                bt = EEPROM.read(alertAdr + 2)&B00111111;
                if (bt == 59) {
                  bt = 0;
                } else
                  bt++;
                EEPROM.write(alertAdr + 2, bt);
              }
              //---------------------------------------------------------------------
              if (cur_item == 11) {
                bt = EEPROM.read(alertAdr + 3)&B00011111;
                if (bt == 23) {
                  bt = 0;
                } else
                  bt++;
                EEPROM.write(alertAdr + 3, bt);
              }
              if (cur_item == 12) {
                bt = EEPROM.read(alertAdr + 4)&B00111111;
                if (bt == 59) {
                  bt = 0;
                } else
                  bt++;
                EEPROM.write(alertAdr + 4, bt);
              }
              //------------------------------------------------------------------------------
            } else if (cur_item == 12) {
              cur_item = 1;
            } else
              cur_item++;
            break;
          case BUTTON_DOWN:
            if (cur_item_edit) {

              if (cur_item == 9) {
                bt = EEPROM.read(alertAdr + 1)&B00011111;
                if (bt == 0) {
                  bt = 23;
                } else
                  bt--;
                EEPROM.write(alertAdr + 1, bt);
              }
              if (cur_item == 10) {
                bt = EEPROM.read(alertAdr + 2)&B00111111;
                if (bt == 0) {
                  bt = 59;
                } else
                  bt--;
                EEPROM.write(alertAdr + 2, bt);
              }
              //---------------------------------------------------------------
              //---------------------------------------------------------------------
              if (cur_item == 11) {
                bt = EEPROM.read(alertAdr + 3)&B00011111;
                if (bt == 0) {
                  bt = 23;
                } else
                  bt--;
                EEPROM.write(alertAdr + 3, bt);
              }
              if (cur_item == 12) {
                bt = EEPROM.read(alertAdr + 4)&B00111111;
                if (bt == 0) {
                  bt = 59;
                } else
                  bt--;
                EEPROM.write(alertAdr + 4, bt);
              }
              //-----------------------------------------------------------------
            } else if (cur_item == 1) {
              cur_item = 12;
            } else
              cur_item--;
            break;
          case BUTTON_OK:
            EEPROM.write(alertAdr + 5, curLoad);
            switch (cur_item) {
              case 8:
                EEPROM.write(alertAdr, (EEPROM.read(alertAdr)^B10000000));
                break;
              case 9:

                cur_item_edit = !cur_item_edit;
                //               if (cur_item_edit)
                //                myGLCD.drawLine(0, 29, 83, 29);

                break;
              case 10:
                cur_item_edit = !cur_item_edit;
                break;
              //------------------------------------------------------------------
              case 11:
                cur_item_edit = !cur_item_edit;
                break;
              case 12:
                cur_item_edit = !cur_item_edit;
                break;
              //------------------------------------------------------------------
              default:
                EEPROM.write(alertAdr, (EEPROM.read(alertAdr) ^ (1 << (cur_item - 1))));
                break;
            }
            break;
          case BUTTON_CANCEL:
            switch (curLoad) {
              case 1:
                //Beli",
                cur_menu = 133;
                cur_item = 1;
                break;
              case 2:
                //"sini"
                cur_menu = 133;
                cur_item = 1;
                break;
              case 3:
                //"`erweni"
                cur_menu = 133;
                cur_item = 1;
                break;
                //                case 4:
                //                  //"CO2"
                //                  cur_menu = 133;
                //                  cur_item = 1;
                //                  break;
                //                case 5:
                //                  //"LED"
                //                  cur_menu = 133;
                //                  cur_item = 1;
                //                  break;

            }
            cur_item_edit = false;
            break;
        }
        myGLCD.update();
        break;
      }
    //      case 132: // Termostat -----------------------------------------------------------------
    //        {
    //          String vv;
    //          if (CoolingEnabled ) {
    //            vv = "ON";
    //          } else {
    //            vv = "OFF";
    //          }
    //          //          EEPROM.write(5, CoolingEnabled);
    //          //          drawMenu("termostat", "temp [" + (String) Temp + "."+(String) Temp_dr + "]", "wentilator " + vv, "min oboroti", "maks oboroti"); //Termostat Aqua T CoolTime minCool maxCool
    //          drawMenu("termostat", "temperatura" , "wentilator " + vv, "min oboroti", "maks oboroti");
    //          naviKey(4);
    //          switch (key) {
    //            case BUTTON_OK:
    //              switch (cur_item) {
    //                case 1:
    //                  cur_menu = 1321;
    //                  cur_item = 0;
    //                  break;
    //                case 2:  //вентилатор
    //                  CoolingEnabled  = !CoolingEnabled ;
    //                  if (CoolingEnabled ) {
    //                    CoolingEnabled = true;
    //                    analogWrite(EXT_COOLING_PIN, minCool);
    //                  } else {
    //                    CoolingEnabled = false;
    //                    digitalWrite(EXT_COOLING_PIN, LOW);
    //                  }
    //                  EEPROM.write(5, CoolingEnabled);
    //                  break;
    //                case 3:  //minCool
    //                  cur_menu = 1323;
    //                  break;
    //                case 4:  //maxCool
    //                  cur_menu = 1324;
    //                  break;
    //              }
    //              break;
    //            case BUTTON_CANCEL:
    //              cur_menu = 13;
    //              cur_item = 2;
    //              cur_item_edit = false;
    //              break;
    //          }
    //          break;
    //        }
    //      case 1321: // Aqua Temperature -----------------------------------------------------------------
    //        {
    //          myGLCD.setFont(SmallFont);
    //          myGLCD.clrScr();
    //
    //          myGLCD.print("temperatura", 1, 0);
    //          //  myGLCD.print(rtc.getTimeStr(FORMAT_SHORT),RIGHT,0);
    //          myGLCD.drawLine(0, 8, 83, 8);
    //          myGLCD.drawLine(0, 9, 83, 9);
    //          myGLCD.setFont(MediumNumbers);
    //          Temp_dr = aquaT % 10;
    //          Temp  = aquaT / 10;
    //          myGLCD.printNumI(Temp, 14, 15, 2);
    //          myGLCD.printNumI(Temp_dr, 41, 15, 1);
    //          myGLCD.drawRect(39, 29, 40, 30);
    //          //  myGLCD.drawRoundRect(0, 38, 83, 47);
    //          //  int k = 80 * (aquaT - 180) / (350 - 180);
    //          //  for (int i = 2; i < k + 2; i++)
    //          //    myGLCD.drawLine(i, 40, i, 46);
    //          myGLCD.update();
    //
    //          // at = aquaT-100;
    //
    //          //          drawIntMenu("temperatura", 18, 35, aquaT); //Aqua Temperat
    //          switch (key) {
    //            case BUTTON_UP:
    //              aquaT++;
    //              aquaT = min(aquaT, 350);
    //              break;
    //            case BUTTON_DOWN:
    //              aquaT--;
    //              aquaT = max(aquaT, 180);
    //              break;
    //            case BUTTON_OK:
    //              at = aquaT - 100;
    //              EEPROM.write(2, at);
    //              cur_menu = 132;
    //              cur_item = 1;
    //              break;
    //            case BUTTON_CANCEL:
    //              //              aquaT = EEPROM.read(2) + 100;
    //              cur_menu = 132;
    //              cur_item = 1;
    //              break;
    //          }
    //          break;
    //        }
    //----------------------Калибровка на сонда за РН----------------------------------
    //      case 135:
    //        {
    //          myGLCD.setFont(SmallFont);
    //          myGLCD.clrScr();
    //          myGLCD.print("kalibrowka", 1, 0);
    //          myGLCD.drawLine(0, 8, 83, 8);
    //          myGLCD.drawLine(0, 9, 83, 9);
    //          //         int pH_d = (pH_new - (int)pH_new) * 100;
    //          myGLCD.printNumI(pH_new, 10, 39, 2, ' ');
    //          myGLCD.printNumI(pH_d, 26, 39, 2, '0');
    //          myGLCD.printNumI(millivolts, 46, 39);
    //
    //          int y1_d = (y1 - (int)y1) * 100;   //Отделяне на дробната част от температурата
    //          myGLCD.printNumI(y1, 20, 15, 2, ' ');
    //          myGLCD.printNumI(y1_d, 33, 15, 2, '0');
    //          myGLCD.printNumI(x1, 50, 15);
    //          int y2_d = (y2 - (int)y2) * 100;   //Отделяне на дробната част от температурата
    //          myGLCD.printNumI(y2, 20, 26, 2, ' ');
    //          myGLCD.printNumI(y2_d, 33, 26, 2, '0');
    //          myGLCD.printNumI(x2, 50, 26);
    //
    //          if ((cur_item == 1) && cur_item_edit)
    //            myGLCD.invertText(true);
    //          myGLCD.print("k1-", 3, 15);
    //          myGLCD.invertText(false);
    //
    //          if ((cur_item == 2) && cur_item_edit)
    //            myGLCD.invertText(true);
    //          myGLCD.print("k2-", 3, 26);
    //          myGLCD.invertText(false);
    //          if (cur_item == 1) {
    //            //    myGLCD.print("k1", 3, 12);
    //            myGLCD.drawRoundRect(0, 13, 83, 24);
    //          }
    //          if (cur_item == 2) {
    //            //    myGLCD.print("k2", 3, 21);
    //            myGLCD.drawRoundRect(0, 24, 83, 35);
    //          }
    //          switch (key) {
    //            case BUTTON_UP:
    //              if (!cur_item_edit) {
    //                cur_item--;
    //                if (cur_item < 1)
    //                  cur_item = 2;
    //              } else {
    //                if (cur_item == 1) {
    //                  y1 = y1 + 0.01;
    //                  if (y1 == 15)
    //                    y1 = 0;
    //                }
    //                if (cur_item == 2) {
    //                  y2 = y2 + 0.01;
    //                  if (y2 == 10)
    //                    y2 = 0;
    //                }
    //              }
    //              break;
    //            case BUTTON_DOWN:
    //              if (!cur_item_edit) {
    //                cur_item++;
    //                if (cur_item > 2)
    //                  cur_item = 1;
    //              } else {
    //                if (cur_item == 1) {
    //                  if (y1 == 0)
    //                    y1 = 15;
    //                  else
    //                    y1 = y1 - 0.01;
    //                }
    //                if (cur_item == 2) {
    //                  if (y2 == 0)
    //                    y2 = 10;
    //                  else
    //                    y2 = y2 - 0.01;
    //                }
    //              }
    //              break;
    //            case BUTTON_OK:
    //              if (cur_item_edit) {
    //                switch (cur_item) {
    //                  case 1:
    //                    EEPROM_float_write(60, y1); //запис на у1 в EEPROM
    //                    EEPROM_int_write(70, millivolts);              //запис на миливолтите за първия еталон
    //                    x1 = EEPROM_int_read(70);
    //                    break;
    //                  case 2:
    //                    EEPROM_float_write(65, y2);
    //                    EEPROM_int_write(75, millivolts);//запис на миливолтите за втория еталон
    //                    x2 = EEPROM_int_read(75);
    //                    break;
    //                }
    //                cur_item_edit = false;
    //              } else
    //                cur_item_edit = true;
    //              break;
    //            case BUTTON_CANCEL:
    //            if (cur_item_edit){
    //              cur_item_edit = false;
    //            }else{
    //              cur_item_edit = false;
    //              cur_menu = 134;
    //              cur_item = 4;
    //          }
    //              break;
    //          }
    //
    //          myGLCD.update();
    //
    //
    //          break;
    //        }
    //        //--------------------------------------------------------------------------------
    //
    //      case 1323: // minCool
    //      {
    //          drawIntMenu("min oboroti", 0, maxCool, minCool); //minCool
    //          switch (key) {
    //            case BUTTON_UP:
    //              minCool++;
    //              minCool = min(minCool, 255);
    //              analogWrite(EXT_COOLING_PIN, minCool);
    //              break;
    //            case BUTTON_DOWN:
    //              minCool--;
    //              minCool = max(minCool, 0);
    //              analogWrite(EXT_COOLING_PIN, minCool);
    //              break;
    //            case BUTTON_OK:
    //              EEPROM.write(3, minCool);
    //              cur_menu = 132;
    //              cur_item = 3;
    //              break;
    //            case BUTTON_CANCEL:
    //              minCool = EEPROM.read(3);
    //              cur_menu = 132;
    //              cur_item = 3;
    //              break;
    //          }
    //          break;
    //        }
    //      case 1324: // maxCool //------------------------------------------
    //        {
    //          drawIntMenu("maks oboroti", 0, 255, maxCool); //maxCool
    //          switch (key) {
    //            case BUTTON_UP:
    //              maxCool++;
    //              maxCool = min(maxCool, 255);
    //              analogWrite(EXT_COOLING_PIN, maxCool);
    //              break;
    //            case BUTTON_DOWN:
    //              maxCool--;
    //              maxCool = max(maxCool, 0);
    //              analogWrite(EXT_COOLING_PIN, maxCool);
    //              break;
    //            case BUTTON_OK:
    //              EEPROM.write(4, maxCool);
    //              cur_menu = 132;
    //              cur_item = 4;
    //              break;
    //            case BUTTON_CANCEL:
    //              maxCool = EEPROM.read(4);
    //              cur_menu = 132;
    //              cur_item = 4;
    //              break;
    //          }
    //          break;
    //        }
    case 131: // System -----------------------------------------------------------------
      {
        //          String pp;
        //          if (PH_sonda ) {
        //            pp = "ON";
        //          } else {
        //            pp = "OFF";
        //          }

        drawMenu("men/", "data i `as", "LCD kontrast", " ", " "); //"System", "Time","Date","LED Contrast","LED Light"
        naviKey(2);
        switch (key) {
          case BUTTON_OK:
            switch (cur_item) {
              case 1:
                cur_menu = 1312;
                cur_item = 0;
                menu_hh = time.hour;
                menu_mi = time.min;
                menu_dd = time.date;
                menu_mm = time.mon;
                menu_yyyy = time.year;
                menu_dow = time.dow;
                break;
              case 2:
                cur_menu = 1314;
                cur_item = 0;
                break;
            }
            break;
          case BUTTON_CANCEL:
            cur_menu = 13;
            cur_item = 0;
            break;
        }
        break;
      }

    case 1312: // Date -----------------------------------------------------------------
      {
        myGLCD.setFont(SmallFont);
        myGLCD.clrScr();

        myGLCD.print("data i `as", 10, 0); //Date
        //          myGLCD.print(rtc.getTimeStr(FORMAT_SHORT), RIGHT, 0);
        myGLCD.drawLine(0, 8, 83, 8);
        myGLCD.drawLine(0, 9, 83, 9);

        if ((cur_item == 1) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.printNumI(menu_dd, 12, 15, 2, '0');
        myGLCD.invertText(false);

        myGLCD.print(".", 24, 15);

        if ((cur_item == 2) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.printNumI(menu_mm, 30, 15, 2, '0');
        myGLCD.invertText(false);

        myGLCD.print(".", 42, 15);

        if ((cur_item == 3) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.printNumI(menu_yyyy, 48, 15, 4, '0');
        myGLCD.invertText(false);

        //          myGLCD.print("p w s ` p s n", 0, 29); //Day of week



        if ((cur_item == 4) && cur_item_edit)
          myGLCD.invertText(true);
        //          myGLCD.print(rtc.getDOWStr(FORMAT_LONG) , 39, 27);
        switch (menu_dow)
        {
          case 1:     myGLCD.print("ponedelnik", CENTER, 27);    break;
          case 2:     myGLCD.print("wtornik", CENTER, 27);        break;
          case 3:     myGLCD.print("srqda", CENTER, 27);          break;
          case 4:     myGLCD.print("`etwyrtyk", CENTER, 27);        break;
          case 5:     myGLCD.print("petyk", CENTER, 27);        break;
          case 6:     myGLCD.print("sybota", CENTER, 27);        break;
          case 7:     myGLCD.print("nedelq", CENTER, 27);    break;
        }
        myGLCD.invertText(false);

        if ((cur_item == 5) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.printNumI(menu_hh, 26, 39, 2, '0');
        myGLCD.invertText(false);
        myGLCD.print(":", 39, 39);

        if ((cur_item == 6) && cur_item_edit)
          myGLCD.invertText(true);
        myGLCD.printNumI(menu_mi, 46, 39, 2, '0');
        myGLCD.invertText(false);


        switch (cur_item) {
          case 1:
            myGLCD.drawRoundRect(11, 13, 25, 23);
            break;
          case 2:
            myGLCD.drawRoundRect(29, 13, 44, 23);
            break;
          case 3:
            myGLCD.drawRoundRect(47, 13, 73, 23);
            break;
          case 4:
            myGLCD.drawRoundRect(10, 25, 73, 35);
            break;
          case 5:
            myGLCD.drawRoundRect(25, 37, 39, 47);
            break;
          case 6:
            myGLCD.drawRoundRect(45, 37, 59, 47);
            break;
        }

        switch (key) {
          case BUTTON_UP:
            if (!cur_item_edit) {
              cur_item++;
              if (cur_item > 6)
                cur_item = 1;
            } else {
              if (cur_item == 1) {
                menu_dd++;
                if (menu_dd > 31)
                  menu_dd = 1;
              }
              if (cur_item == 2) {
                menu_mm++;
                if (menu_mm > 12)
                  menu_mm = 1;
              }
              if (cur_item == 3) {
                menu_yyyy++;
              }
              if (cur_item == 4) {
                menu_dow++;
                if (menu_dow > 7)
                  menu_dow = 1;
              }
              if (cur_item == 5) {
                menu_hh++;
                if (menu_hh == 24)
                  menu_hh = 0;
              }
              if (cur_item == 6) {
                menu_mi++;
                if (menu_mi == 59)
                  menu_mi = 0;
              }

            }
            break;
          case BUTTON_DOWN:
            if (!cur_item_edit) {
              cur_item--;
              if (cur_item < 1)
                cur_item = 6;
            } else {
              if (cur_item == 1) {
                menu_dd--;
                if (menu_dd < 1)
                  menu_dd = 31;
              }
              if (cur_item == 2) {
                menu_mm--;
                if (menu_mm < 1)
                  menu_mm = 12;
              }
              if (cur_item == 3) {
                menu_yyyy--;
                menu_yyyy = max(menu_yyyy, 2013);
              }
              if (cur_item == 4) {
                menu_dow--;
                if (menu_dow < 1)
                  menu_dow = 7;
              }
              if (cur_item == 5) {
                if (menu_hh == 0)
                  menu_hh = 23;
                else
                  menu_hh--;
              }
              if (cur_item == 6) {
                if (menu_mi == 0)
                  menu_mi = 59;
                else
                  menu_mi--;
              }
            }
            break;
          case BUTTON_OK:
            if (cur_item_edit) {
              switch (cur_item) {
                case 1:
                  rtc.setDate(menu_dd, time.mon, time.year);   // Set the date
                  break;
                case 2:
                  rtc.setDate(time.date, menu_mm, time.year);   // Set the date
                  break;
                case 3:
                  rtc.setDate(time.date, time.mon, menu_yyyy);   // Set the date
                  break;
                case 4:
                  rtc.setDOW(menu_dow);     // Set Day-of-Week
                  break;
                case 5:
                  rtc.setTime(menu_hh, time.min, 0);     // Set the time to 12:00:00 (24hr format)
                  break;
                case 6:
                  rtc.setTime(time.hour, menu_mi, 0);     // Set the time to 12:00:00 (24hr format)
                  break;
              }
              cur_item_edit = false;
            } else
              cur_item_edit = true;
            break;
          case BUTTON_CANCEL:
            cur_item_edit = false;
            cur_menu = 131;
            cur_item = 1;
            break;
        }
        myGLCD.update();
        break;
      }
    /*      case 1313: // LED Contrast -----------------------------------------------------------------
            {

            }
    */
    case 1314: // Podsvetka!!!//LED Light -----------------------------------------------------------------
      {
        drawIntMenu("LED kontrast", 45, 72, LCDContrast);
        switch (key) {
          case BUTTON_UP:
            LCDContrast++;
            LCDContrast = min(LCDContrast, 72);
            break;
          case BUTTON_DOWN:
            LCDContrast--;
            LCDContrast = max(LCDContrast, 45);
            break;
          case BUTTON_OK:
            EEPROM.write(0, LCDContrast);
            cur_menu = 131;
            cur_item = 2;
            break;
          case BUTTON_CANCEL:
            //                  LCDContrast = EEPROM.read(0);
            cur_menu = 131;
            cur_item = 2;
            break;
        }
        myGLCD.setContrast(LCDContrast);
        break;
      }

    default:
      cur_menu = 0;
      cur_item = 0;
  }

  loopTime = currentTime;
}

void first_setrup() {
  //  -------------------------------------------------------------------------------
  // first setup
  int l;
  int cl = 1;
  byte bl;
  for (l = 100; l < 600; l = l + 10) {
    byte l1 = EEPROM.read(l);
    bl = (EEPROM.read(l + 1)&B00011111);
    if (bl >= 0) {
      EEPROM.write(l + 1, 0);
      EEPROM.write(l + 5, cl);
      if (cl < 5) {
        cl++;
      } else {
        cl = 1;
      }

    } else {
      EEPROM.write(l + 1, 0);
    }
    bl = (EEPROM.read(l + 0)&B00011111);
    if (bl >= 0 ) {
      EEPROM.write(l + 2, 0);
    } else {
      EEPROM.write(l + 2, 0);
    }
    bl = (EEPROM.read(l + 0)&B00011111);
    if (bl >= 0) {
      EEPROM.write(l + 3, 0);
    } else {
      EEPROM.write(l + 3, 0);
    }
    bl = (EEPROM.read(l + 0)&B00011111);
    if (bl >= 0) {
      EEPROM.write(l + 4, 0);
    } else {
      EEPROM.write(l + 4, 0);
    }
    //        EEPROM.write(l + 1, 0);
    //        EEPROM.write(l + 2, 0);
    //        EEPROM.write(l + 3, 0);
    //        EEPROM.write(l + 4, 0);
  }
  //EEPROM.write(90, 1);
  //        Serial.print(F("A: "));
  //        Serial.print(a);
  //        Serial.print(F(" T_ON: "));
  //        Serial.print(EEPROM.read(a + 1));
  //        Serial.print(F(":"));
  //        Serial.print(EEPROM.read(a + 2));
  //        Serial.print(F(" T_OFF: "));
  //        Serial.print(EEPROM.read(a + 3));
  //        Serial.print(F(":"));
  //        Serial.println(EEPROM.read(a + 4));
  //        Serial.print(F("B1"));
  //        Serial.println(b1);
  //        T_ON = EEPROM.read(a + 1) * 60 + EEPROM.read(a + 2);
  //        T_OFF = EEPROM.read(a + 3) * 60 + EEPROM.read(a + 4);
  //        T_cur = time.hour * 60 + time.min;

}
