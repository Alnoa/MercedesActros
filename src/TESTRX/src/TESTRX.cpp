#include <Arduino.h>
#include <Truckbee.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define DEBUG
// Def des PINS

#define Pin_veilleuses 						0
#define Pin_feux_de_croisements 	1
#define Pin_phares 								2
#define Pin_cligno_droit 					3
#define Pin_cligno_gauche 				4
#define Pin_longues_vues_hautes 	5
#define Pin_antibrouillards 			6
#define Pin_feux_stop 						7
#define Pin_feux_de_recul 				8
#define Pin_gyrophares 						9

// Def des variables locales
unsigned long currentMillis;
unsigned long prev_timer=0;
unsigned int timer= 1000;
//def des objets
TruckBee RxTruck(9600,true);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//methodes locales
int trait_poucentage(int data){
  return (((data*9)/1024)+48);
}
void trait_lumieres(){
  if(RxTruck.getVeilleuse()){
    pwm.setPWM(Pin_veilleuses,0,4095);
  }else{pwm.setPWM(Pin_veilleuses,4095,4095);}
  if(RxTruck.getFeux_de_croisements()){
    pwm.setPWM(Pin_feux_de_croisements,0,4095);
  }else{pwm.setPWM(Pin_feux_de_croisements,4095,4095);}
}




void setup() {
  pwm.begin();
  pwm.setPWMFreq(1600);
  pwm.setPWM(Pin_feux_stop,0,4095);
  RxTruck.Etablir_liason(true);
  pwm.setPWM(Pin_feux_stop,4095,4095);
}

void loop() {
  RxTruck.Recoie();
  trait_lumieres();
}
