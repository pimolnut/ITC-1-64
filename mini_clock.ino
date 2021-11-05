#include <EEPROM.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
 
Adafruit_SSD1306 OLED(OLED_RESET);

#define ldr A3

int speakerPin = 4;

int8_t hour = 0;
int8_t minute = 0;
int8_t second = 0;

int8_t timealarm_h;
int8_t timealarm_m;
bool setalarm = false;

int button1 = 2; //set hour
int button2 = 1; //set minute
int button3 = 3; //set alarm
int button4 = 5; //save to EEPROM

int currentSw1 = 0;
int currentSw2 = 0;
int currentSw3 = 0;

int sensor_ldr;

void setup()
 {
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);

  pinMode(speakerPin,OUTPUT);
  
  EEPROM.get(0,hour);
  EEPROM.get(1,minute);
  EEPROM.get(2,second);
  
  OLED.clearDisplay(); 
  OLED.setTextColor(WHITE);
  OLED.drawRect(0,0,128,32,WHITE);
  OLED.display();
  delay(150);
  OLED.clearDisplay(); 
 }

void loop()
 {
  
  // LDR 
  sensor_ldr = analogRead(ldr);
  Serial.println(sensor_ldr);
  if(sensor_ldr >= 500)
  {
    Light_mode();
  }
  else
  {
    Dark_mode();  
  }

  //sw1 set minute
  if(digitalRead(button1) == LOW)
  {
    if((millis()-currentSw1) >= 900)
    {
        if(!setalarm)
        {
          minute = minute+1;
          if(minute>=60) 
          {
           minute = 0; 
          }
        }else
        {
          timealarm_m = timealarm_m +1;
          if(timealarm_m>=60)
          {
            timealarm_m = 0;
          }
        }
     currentSw1 = millis();
    }
  }

  //sw2 set hour
  if(digitalRead(button2) == LOW)
  {
    if((millis()-currentSw2) >= 900)
    {
      if(!setalarm)
      {
        hour = hour+1;
        if(hour>23) 
        {
         hour = 0; 
        }
      }else
        {
          timealarm_h = timealarm_h +1;
          if(timealarm_h>23)
          {
            timealarm_h = 0;
          }
        }
     currentSw2 = millis();
    }
  }

  //set time alarm
  if(digitalRead(button3) == LOW)
  {
    if((millis()-currentSw3) >= 900)
    {
     setalarm =! setalarm;
     currentSw3 = millis();
    }
  }

  //save to EEPROM
  if(digitalRead(button4) == LOW)
  {
   EEPROM.update(0,hour);
   EEPROM.update(1,minute);
   EEPROM.update(2,second);
  }

  //alarm
  if( hour == timealarm_h && minute == timealarm_m)
  {
    tone(speakerPin,70,100);
  }
  
  OLED.clearDisplay();
  OLED.setCursor(10,20);
  OLED.print("alarm");
  OLED.setCursor(45,20);
   if(timealarm_h<10)
  {
    OLED.print(0);
  }
  OLED.print(timealarm_h);
  OLED.print(":");
  OLED.setCursor(63,20); 
  if(timealarm_m<10)
  {
    OLED.print(0);
  }
  OLED.print(timealarm_m);

  //counttime
  OLED.setCursor(10, 7);
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
  OLED.drawRect(0,0,128,32,WHITE);
  second = second + 1;
   if(hour<10)
  {
    OLED.print(0);
  }
  OLED.print(hour);
  OLED.print(":");
  if(minute<10)
  {
    OLED.print(0);
  }
  OLED.print(minute);
  OLED.setTextSize(1);
  OLED.print(":");
   if(second<10)
  {
    OLED.print(0);
  }
  OLED.print(second);
  OLED.setTextSize(1);
  OLED.display();
  delay(500);
  
  if (second == 60)
  {
    second = 0;
    minute = minute + 1;
  }
  if (minute == 60)
  {
    minute = 0;
    hour = hour + 1;
  }
  if (hour == 24)
  {
    hour = 1;
  }
  if(hour == 8)
  {
    OLED.setCursor(87, 7);
    OLED.print("GOOD");
    OLED.setCursor(80, 17);
    OLED.print("MORNING");
  }
  if(hour == 12)
  {
    OLED.setCursor(87, 7);
    OLED.print("GOOD");
    OLED.setCursor(87, 17);
    OLED.print("ATN");
  }
  if(hour == 18)
  {
    OLED.setCursor(87, 7);
    OLED.print("GOOD");
    OLED.setCursor(80, 17);
    OLED.print("EVENING");
  }
  OLED.display();
  delay(150);
 }

void Dark_mode()
{
  OLED.setTextColor(WHITE);
  OLED.setTextSize(1);
}
void Light_mode()
{
  OLED.fillScreen(WHITE);
  OLED.setTextColor(BLACK);
  OLED.setTextSize(1);
}
