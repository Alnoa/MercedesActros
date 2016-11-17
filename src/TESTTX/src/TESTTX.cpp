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
unsigned long currentMillis;
bool buttonState;
int lastButtonState = LOW;
char result[6];
int temp_incr_phares=0;
unsigned long prev_timer=0;
unsigned int timer= 1000;
//def des objets
TruckBee TxTruck(9600,true);
LiquidCrystal_I2C lcd(0x27,20,4);  //addresse 0x27,16chars,2line

//methodes locales
int trait_poucentage(int data){
  return (((data*9)/1024)+48);
}
void lecture_des_entrees_analog(){
  //  TxTruck.direction(map(tempX,0,100,-100,100));
  TxTruck.direction(map(analogRead(axeXJoy),0,1023,-100,100));
  TxTruck.accel(map(analogRead(axeYJoy)-583,-583,440,-100,100));
}
void lecture_des_entrees_digitales(){
  TxTruck.klaxon(!digitalRead(bouttonJoy));
  if(!digitalRead(bouttonJoy)){
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
  lcd.print(" ");
  lcd.setCursor(0,3);
  lcd.print("klaxon: ");
  lcd.print(TxTruck.getKlaxon());
  if(TxTruck.getKlaxon()){
    if(currentMillis-prev_timer>=timer){
      prev_timer=currentMillis;
      tone(3,2900);
    }else{
      noTone(3);
    }
  }else{noTone(3);}
  lcd.print(" ");
  lcd.print(temp_incr_phares);
}

void setup() {
  pinMode(bouttonJoy, INPUT_PULLUP);
  pinMode(axeYJoy,INPUT);
  pinMode(axeXJoy,INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Actros TX");
  delay(200);
  TxTruck.Etablir_liason(true);
}

void loop() {
  currentMillis = millis();
  lecture_des_entrees_analog();
  lecture_des_entrees_digitales();
  affichage_lcd();
  TxTruck.Envoie();
  //delay(50);
  //lcd.clear();
}
