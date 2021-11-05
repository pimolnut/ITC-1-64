#include<Wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 OLED(OLED_RESET) ;
int speakerPin = 8;

double xstart = 54; 
int ystart = 24; 
double xfinish = 74; 
int yfinish = 24; 

int xBall = 54; //position ball
int yBall = 20; 

int button1 = 2;
int button2 = 3;

double yBallbouncing = 1; //ค่าy
double xBallbouncing = 0; //ค่าx

void ball(){
  if(xBall>=xstart && xBall<=xfinish&&yBall == 24) //check ball 
  {
    xBallbouncing=(xBall-(xfinish/2))/21.f;
    yBallbouncing=-1;  //up-down
    tone(speakerPin,70,100);
  }
  if(yBall<1)
  {
    yBallbouncing=-yBallbouncing; 
  }
  if(xBall<1||xBall>126){
    xBallbouncing=-xBallbouncing; //x
  }
  yBall += yBallbouncing;
  xBall += xBallbouncing*2; // update
  OLED.drawPixel(xBall,yBall,WHITE);

}


void drawline()
{
  OLED.clearDisplay();
  OLED.drawRect(0,0,128,32,WHITE);
  OLED.drawLine(xstart,ystart,xfinish,yfinish,WHITE);
  ball();
  OLED.display() ;
  if(digitalRead(button1) == LOW)
  {
    OLED.clearDisplay();
    if(xfinish <= 126) 
  {
      xstart = xstart+10;
      xfinish = xfinish+10;
  }
    OLED.drawRect(0,0,128,32,WHITE);
    OLED.drawLine(xstart,ystart,xfinish,yfinish,WHITE);
    OLED.drawPixel(xBall,yBall,WHITE);
    OLED.display() ;
  }
  if(digitalRead(button2) == LOW)
  {
    OLED.clearDisplay();
    if(xfinish >= 0) {
    xstart = xstart-10;
    xfinish = xfinish-10;
  }
  OLED.drawRect(0,0,128,32,WHITE);
  OLED.drawPixel(xBall,yBall,WHITE);
  OLED.drawLine(xstart,ystart,xfinish,yfinish,WHITE);
  OLED.display() ;
  }
}
void setup() 
{
  Serial.begin(115200) ;
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C) ;
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);

  OLED.clearDisplay(); 
  OLED.setTextColor(WHITE);
  OLED.drawRect(0,0,128,32,WHITE);
  OLED.setCursor(10,10);
  OLED.setTextSize(2);
  OLED.println("WELCOME!!");
  OLED.display();
  tone(speakerPin,70,100);
  delay(150);
  OLED.clearDisplay(); 
}
void loop() 
{ 
  drawline();
  if(yBall > 29) 
  {
    OLED.clearDisplay();
    OLED.drawRect(0,0,128,32,WHITE);
    OLED.setCursor(10,10);
    OLED.setTextColor(WHITE);
    OLED.setTextSize(2);
    OLED.println("Game Over");
    OLED.display();
    delay(5000);
  }
 }
