
//imports infrared thermometer class
#include<SparkFunMLX90614.h>

//imports LCD class
#include <LCD5110_Graph.h>

//I2C library, required for MLX90614
#include <Wire.h> 
int buttonState,buttonPushes=0;
String stringBS;
const int LED = 2, BUTTON = 6, LASER = 3, PUSHED = 0, NOT_PUSHED = 1;
extern uint8_t SmallFont[];
extern uint8_t LargeFont[];
extern uint8_t arduino_logo[];
extern unsigned char TinyFont[];
uint8_t* bm;
boolean intro = true;

//instantiating the LCD class
LCD5110 LCD(8,9,10,12,11);

//instantiating the IRTherm class
IRTherm therm;

void setup() {
  
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LASER, OUTPUT);
  Serial.begin(9600);

  // Initialize thermal IR sensor
  therm.begin(); 

  // Set the library's units to Farenheit
  therm.setUnit(TEMP_F); 
  
  //initializes the LCD screen
  LCD.InitLCD();
  LCD.setFont(TinyFont);
  randomSeed(analogRead(7));
  
}

void loop() {
 
 //only occurs at start up
 //classy logos for looks
 while (intro){
  LCD.clrScr();
  LCD.drawBitmap(0, 0, arduino_logo, 84, 48);
  LCD.update();
  LCD.clrScr();
  delay (2000);
  LCD.print("Cold Fusion", CENTER, 0);
  LCD.print("Temperature Sensing", CENTER, 10);
  LCD.print("Device", CENTER, 20);
  LCD.drawRect(28, 18, 56, 28);
  for (int i=0; i<6; i++)
  {
   LCD.drawLine(57, 18+(i*2), 83-(i*3), 18+(i*2));
   LCD.drawLine((i*3), 28-(i*2), 28, 28-(i*2));
  }
  LCD.setFont(TinyFont);
  LCD.print("(C) 2018 by", CENTER, 36);
  LCD.print("Jake Chandler", CENTER, 42);
  LCD.update();
  delay(4000);
  LCD.clrScr();
  LCD.setFont(SmallFont);
  intro = false;
 }
  if (therm.read()){
  LCD.clrScr();
  buttonState = digitalRead(BUTTON);

 if (buttonState==PUSHED){
     digitalWrite(LASER,HIGH);
     LCD.clrScr();
     stringBS = "PUSHED";
     Serial.print("Object: " + String(therm.object(), 2));
     Serial.write('°'); // Degree Symbol
     //displays message
     LCD.print("obj. temp(F): ",CENTER- (CENTER*(1/2)), 0);
     LCD.printNumI(therm.object(), CENTER - (CENTER*(1/2)),20);
     LCD.update();
     LCD.setFont(SmallFont);
     if (therm.object()>=68 && therm.object()<=77){
        LCD.print("IN RANGE",CENTER- (CENTER*(1/2)), 40);
        LCD.update();
        digitalWrite(LED,HIGH);
        delay(1000); 
     }
     else{
        LCD.setFont(TinyFont);
        LCD.print("NOT IN RANGE",CENTER- (CENTER*(1/2)), 40);
        LCD.update();
        LCD.setFont(SmallFont);
        digitalWrite(LED, LOW);
        digitalWrite(LASER,LOW);
     }
 }
 else {
     digitalWrite(LASER,LOW);
     digitalWrite(LED, LOW);
     LCD.clrScr();
     LCD.print("Room",CENTER- (CENTER*(1/2)), 0);
     LCD.print("Temperature:",CENTER- (CENTER*(1/2)), 20);
     LCD.update();
     LCD.printNumI(therm.ambient(), CENTER - (CENTER*(1/2)),40);
     LCD.update();
     
 }
  Serial.println(stringBS);
  } 
}
