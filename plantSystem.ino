
//DHT temp and humidity sensor
#include "DHT.h"
#define DHTPIN 13     // S pin connected to DP13
#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

// LCD
// LCD RS=3, EN=4, DS4=5, DS5=6, DS6=7, DS7=8
#include <LiquidCrystal.h>
LiquidCrystal lcd( 3, 4, 5, 6, 7, 8);

//Vibe motor DRV
#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

uint8_t effect1 = 3;
uint8_t effect2 = 15;
boolean buzzedOnce = false;
boolean buzzedAgain = false;

//Photoresistor

int photoResistorValue = 0;
int photoResistorPin = 0;
int ledPin = 10;
int brightness = 0;

//Capacitive Sensor
#include <CapacitiveSensor.h>
CapacitiveSensor   cs_12_2 = CapacitiveSensor(12,2);        

//Servo
#include <Servo.h>
Servo myServo;
int pos;



void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16,2);
  dht.begin();
  drv.begin();
  
  drv.selectLibrary(1);
  
  // I2C trigger by sending 'go' command 
  // default, internal trigger when sending GO command
  drv.setMode(DRV2605_MODE_INTTRIG); 

  pinMode(ledPin, OUTPUT);

  cs_12_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 
  pos=90;
  
}

void loop() {
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  // set the cursor to (0,0):
  lcd.setCursor(0, 0);
  // print from 0 to 9:
  //Serial.println(h);
  //Serial.println(t);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");
  // set the cursor to (16,1):
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");
  delay(500);

  //Read value of senor and store it
  photoResistorValue = analogRead(photoResistorPin);


  //print sensor value
  Serial.println(photoResistorValue);
  delay(10);
  
 
 

   if(t>35 || h<20){
     if(buzzedOnce == false){
        buzz(effect1);
        buzzedOnce = true;
        for(int i = 0; i< 20; i++){ //Need to repalce random number 20 with a definite time decided by the user - Future goals
            digitalWrite(ledPin, HIGH);
            delay(500);
            digitalWrite(ledPin, LOW);
            delay(500);
        }
     }
     if(buzzedAgain == false){ // Need to enter a time gap between both the buzz effects - Future goals
        if(buzzedOnce = true){
          buzz(effect2);
          buzzedAgain = true;
        }
      }
   }
   
   
   if(photoResistorValue >600 && photoResistorValue <800){
      if(buzzedOnce == false){
        buzz(effect1);
        buzzedOnce = true;
        for(int i = 0; i< 20; i++){ //Need to repalce random number 20 with a definite time decided by the user - Future goals
            digitalWrite(ledPin, HIGH);
            delay(500);
            digitalWrite(ledPin, LOW);
            delay(500);
        }
     }
     if(buzzedAgain == false){ // Need to enter a time gap between both the buzz effects - Future goals
        if(buzzedOnce = true){
          buzz(effect2);
          buzzedAgain = true;
        }
      } 
      
   }
   
   //capSense and Servo
   long start = millis();
    long total1 =  cs_12_2.capacitiveSensor(30);
    

    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);                  // print sensor output 1
    Serial.print("\n");

    delay(10);                             // arbitrary delay to limit data to serial port 
    if(total1>0){
      myServo.attach(9);
      
      
      //myServo.write(90);
      //Following code was working at one point(video of which is also attached) but isn't anymore due to reason unknown - servo not going in a fluid manner but in a jerk
      for(pos = 90; pos>=0; pos-=1)    
      {
        Serial.println(pos);
        myServo.write(pos);
        delay(100);
      }
      for(pos = 0; pos <= 90; pos += 1) 
      {
        Serial.println(pos);
        myServo.write(pos);
        delay(100);
      }
      
                         
      delay(500);
    }
  
  

  
}

void buzz(uint8_t effect){
  //Serial.print("Effect #"); Serial.println(effect);
  // set the effect to play
  drv.setWaveform(0, effect);  // play effect 
  drv.setWaveform(1, 0);       // end waveform

  // play the effect!
  drv.go();

  // wait a bit
  delay(500);
}

