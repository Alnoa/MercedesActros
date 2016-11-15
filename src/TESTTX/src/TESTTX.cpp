#include <Arduino.h>
#include <Truckbee.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DEBUG
// Def des PINS
#define bouttonJoy  22
#define axeXJoy A0
#define axeYJoy A1

// Def des variables locales
bool buttonState;
int lastButtonState = LOW;
unsigned int calibrage_joyX,calibrage_joyY;
char result[6];
int temp_incr_phares=0;
//def des objets
TruckBee TxTruck(9600,true);
LiquidCrystal_I2C lcd(0x27,20,4);  //addresse 0x27,16chars,2line

//methodes locales

void lecture_des_entrees_analog(){
  TxTruck.direction(map(analogRead(axeXJoy)-calibrage_joyX,0,1023,-100,100));
  TxTruck.accel(map(analogRead(axeYJoy)-calibrage_joyY,0,1023,-100,100));
}
void lecture_des_entrees_digitales(){
  TxTruck.klaxon(digitalRead(bouttonJoy));
  if(digitalRead(bouttonJoy)){
    temp_incr_phares++;
    if(temp_incr_phares>3){
      temp_incr_phares=0;
    }
  }
  TxTruck.veil_crois_phares(temp_incr_phares);
}

void affichage_lcd(){
  lcd.setCursor(0,1);
  lcd.print("dir: ");
  //sprintf(result,"%03d",TxTruck.getDirection());
  lcd.print(int(TxTruck.getDirection()));
  lcd.print(result);
  lcd.setCursor(0,2);
  lcd.print("acc: ");
  lcd.print(TxTruck.getTraction());
  lcd.setCursor(0,3);
  lcd.print("klaxon: ");
  lcd.print(TxTruck.getKlaxon());
}

void setup() {
  pinMode(bouttonJoy, INPUT_PULLUP);
  pinMode(axeYJoy,INPUT);
  pinMode(axeXJoy,INPUT);
  calibrage_joyX=analogRead(axeXJoy);
  calibrage_joyY=analogRead(axeYJoy);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Actros TX");
  delay(200);
  TxTruck.Etablir_liason(true);
}

void loop() {
  lecture_des_entrees_analog();
  lecture_des_entrees_digitales();
  affichage_lcd();
  TxTruck.Envoie();
  //delay(20);
}
