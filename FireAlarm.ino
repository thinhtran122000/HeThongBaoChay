
#include <Sim800l.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>        
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "DHT.h"                 
#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
#define buzzer 8
const int DHTPIN = 9;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
Sim800l Sim800l;
String textSms , numberSms;
uint8_t index1;
String text1 , text2, text3;
int demSms = 0;
//...........................................................
void setup() {
  Serial.begin(9600);
  _buffer.reserve(100);
  sim.begin(9600);
  Sim800l.begin();
  dht.begin();
  lcd.init();
  lcd.backlight();
  text1 = String("Temperature: ");
  text2 = String(" oC, Humidity: ");
  text3 = String(" %");
  pinMode(buzzer, OUTPUT);
  lcd.setCursor(1, 0);
  lcd.print("CONNECTING");
  chaychu();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print("Humidity   :");
  Sim800l.delAllSms();
  Serial.println("<<Connected>>");
}
//...........................................................
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.setCursor(0, 1);
  lcd.print("Humidity   :    ");
  lcd.setCursor(12, 0);
  lcd.print(t);
  lcd.setCursor(12, 1);
  lcd.print(h);
  text1 += t;
  text1 += text2;
  text1 += h;
  text1 += text3;
  Serial.println(text1);
  if (dht.readTemperature() > 35) {
    lcd.setCursor(0, 1);
    lcd.print("WARNING!!!!      ");
    text1 = "FIRE ALERT";
    digitalWrite(buzzer, HIGH);
    Serial.println(text1);
    numberSms = "+84768004506";
    SendMessage();
    Serial.println("Message Sent");
    //demSms = 1;
    lcd.setCursor(0, 1);
    lcd.print("     WARNING    ");
    while (1) {
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      delay(100);
      t = dht.readTemperature();
      lcd.setCursor(12, 0);
      lcd.print(t);
      if (t <= 35) {
        digitalWrite(buzzer, LOW);
        goto step1;
      }
    }
  }
step1:
  textSms = Sim800l.readSms(1);
  Serial.println("Ready");
  textSms = Sim800l.readSms(1);
  numberSms = Sim800l.getNumberSms(1);
  if (textSms.indexOf("KTHT") != -1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Check the system");
    Serial.println("CHECK KTHT");
    Serial.println(numberSms);
    Serial.println(text1);
    SendMessage();
    Serial.println("Message Sent");
    Sim800l.delAllSms();
  }
  text1 = "Temperature: ";
}
//...........................................................
void SendMessage()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  sim.println("AT+CMGS=\"" + numberSms + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = text1;
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(5000);
}
//...........................................................
void chaychu()
{
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(150);
  }
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(150);
  }
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(150);
  }
}
