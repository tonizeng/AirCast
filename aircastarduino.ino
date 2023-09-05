#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SH1106.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <DHT.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     4  
Adafruit_SH1106 display(OLED_RESET);

#define sensor    A0       // Analog pin 0
#define DHTPIN  2          // Digital pin 2
#define DHTTYPE DHT11      // DHT 11

int gasLevel  = 0;         
String quality =""; 
DHT dht(DHTPIN,  DHTTYPE);

String sendHumidity()
{
  float h = dht.readHumidity();
  String humidity = "RH    :   " + String(h) + " %";
  return humidity;
}

String sendTemp()
{
  float t = dht.readTemperature();
  String temp = "Temp  :   " + String(t) + " C";
  return temp;
}

String air_sensor()
{
  gasLevel = analogRead(sensor);

  if(gasLevel<181){
    quality = "GOOD!";
  }
  else if (gasLevel >181 && gasLevel<225){
    quality =  "POOR!";
  }
  else if (gasLevel >225 && gasLevel<300){
    quality  = " BAD!";
  }
    else if (gasLevel >300 && gasLevel<350){
    quality  = "VERY BAD!";
  }
    else{
    quality = "TOXIC!";   
}
  return quality;
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor,INPUT);
  dht.begin();

  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  display.setTextSize(2);
  display.setCursor(50, 10);
  display.println("Air");
  display.setTextSize(1);
  display.setCursor(23, 30);
  display.println("Quality monitor");
  display.display();
  delay(1200);
  display.clearDisplay();

}

void loop() {
// only one mode can be active at a time - other needs to be commented out
// 1) connected mode: device is connected to a laptop and data is visualized real time on the screen
  String dataToSend = "Gas:" + String(gasLevel) +
                      ",Quality:" + air_sensor() + 
                      ",Temp:" + String(dht.readTemperature()) +
                      ",Humidity:" + String(dht.readHumidity())
                      + "\n"
                      ;

  Serial.print(dataToSend);

// 2) standalone mode: device operates independently of laptop and data is visualized real time on the device's display
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,18);
  display.println("Air Quality:" + air_sensor());

  display.setTextSize(1);
  display.setCursor(1, 40);
  display.println(sendTemp());

  display.setCursor(114, 40);
  display.println("");

  display.setCursor(1,  53);
  display.println(sendHumidity());
  display.setCursor(114, 53);
  display.println("");

  display.display();
  delay(100000);

}


