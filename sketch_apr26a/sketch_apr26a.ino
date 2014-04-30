#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

uint8_t i=0;  //Button Code

//Threshold Variables
 int v1 = 0; int v2 = 0; int v3 = 0; int v4 = 0; int v5 = 0;
 int v6 = 0; int v7 = 0; int v8 = 0; int v9 = 0; int v10 = 0;
 
//Heart Rate Determination Variables
 int t1 = 0; int t2 = 0;
 int HTime1 = 0; int HTime2 = 0;
 
 //Hold values that indicate is Voltage<Threshold
 int a1 = 0; int a2 = 0; int a3 = 0; int a4 = 0; int a5 = 0;
 int a6 = 0; int a7 = 0;
 
 //Holds heart rate values
 int HR = 0; int HR1 = 0; int HR2 = 0; int HR3 = 0; int HR4 = 0;
 int HR5 = 0; int HR6 = 0; int HR7 = 0; int HR8 = 0; int HR9 = 0;
 int HR10 = 0; int HRAverage = 0;
 
 //Amplitude of signal variable
 int Amp = 0;
 
 //Heart Character
 byte heart[8] = {
  B01010,
  B10101,
  B10101,
  B10001,
  B10001,
  B01010,
  B00100
};
 //Zero Character
 byte zero[0] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

 //Button Status
 int k = 0; 
 
void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.setBacklight(GREEN);
}

void loop(){
  
  //Voltage Read
  int Voltage = analogRead(A1);
  
  //Threshold Determination
  v1 = v2; v2 = v3; v3 = v4; v4 = v5; 
  v5 = v6; v6 = v7; v7 = v8; v8 = v9; v9 = v10;
  v10 = Voltage;
  int Threshold = (v1+v2+v3+v4+v5+v6+v7+v8+v9+v10)/10;
  
  //Is Voltage Less than Threshold?
  int Read = Voltage<Threshold;
  //Serial.println(Read);
  a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = Read;
  a6 = a7;
  a7 = ((a1 == a2) && (a2 == a3) && (a3 == a4) && (a4 == a5) && (a5 == 1));
  //Serial.print(a6);
  //Serial.print(a7);
  
  if (a6 != a7){
    //'Out' of pulse
    if (a6 == 1){
      t2 = millis();
      HTime1 = HTime2;
      HTime2 = (t2 + t1)/2;
      //Serial.println(1);
      HR = 60000/(HTime2 - HTime1);
      
      if (HR<10 || HR>240){
      }
      else {
        HR1 = HR2; HR2 = HR3; HR3 = HR4; HR4 = HR5;
        HR5 = HR6; HR6 = HR7; HR7 = HR8; HR8 = HR9;
        HR9 = HR10; HR10 = HR;
        HRAverage = (HR1+HR2+HR3+HR4+HR5+HR6+HR7+HR8+HR9+HR10)/10;
        Serial.print(byte(8));
        Serial.print("Heart Rate = ");
        Serial.println(HR);
        Serial.print("Average Heart Rate = ");
        Serial.println(HRAverage);  
        analogWrite(5,0);  
        
        //lcd heartbeat
        lcd.setCursor(15,1);
        lcd.print(byte(8));
      }
      
      //LCD Printouts
      if (k == 0) {
        lcd.setCursor(5,0);
        lcd.print(HR);
      }
      else if (k == 1) {
        lcd.setCursor(11,0);
        lcd.print(millis());
        lcd.setCursor(10,1);
        lcd.print(HRAverage);
      } 
      else {
        lcd.setCursor(9,0);
        lcd.print(Threshold);
        lcd.setCursor(8,1);
        lcd.print(Voltage);
      }
      
    }
    //'In' of pulse
    else {
      t1 = millis();
      //Serial.println(2);
      Amp = 255*(Threshold - Voltage)/10;  
      Serial.print("Amplitude = ");
      Serial.println(Amp); 
      analogWrite(5,Amp);
    }
    
  }
  
  delay(20);
  //blanks out lcd heartbeat
  lcd.setCursor(15,1);
  lcd.print(byte(0));
  
//Button Code & LCD Printouts
  if (buttons) {
    if (buttons && BUTTON_UP) {
      lcd.clear();
      lcd.print("HR = ");
      k = 0;
    }
    if (buttons && BUTTON_DOWN) {
      lcd.clear();
      lcd.print("Run Time = ");
      lcd.setCursor(0,1);
      lcd.print("Mean HR = ")
      k = 1;
    }
    if (buttons && BUTTON_LEFT) {
      lcd.clear();
      lcd.print("Thres. = ");
      lcd.setCursor(0,1);
      lcd.print("Volt. = ");
      k = 2;
    }
}
