#include "truckbee.h"
#if (ARDUINO < 100)
#include "WProgram.h"
#else
#include "Arduino.h"
#endif
#include <arduino.h>
#define DEBUG
/**
* adresse de la RAM
*/
#define adrmemtab_veilleuses 0
#define adrmemtab_feux_de_crois 1
#define adrmemtab_phares 2
#define adrmemtab_clignotant_droit 3
#define adrmemtab_clignotant_gauche 4
#define adrmemtab_longues_vues_hautes 5
#define adrmemtab_antibrouillards 6
#define adrmemtab_gyrophares 7
#define adrmemtab_klaxon 8



/**
* speed_xbee = int = vitesse du port serie xbee
* mode = bool = true==mode emetteur || false==mode recepteur
*/
TruckBee::TruckBee(int speed_xbee,bool mode){
  memtab[0] = 0x00;
  memtab[1] = 0b00000000;
  memtab[2] = 0b00000000;
  memtab[3] = 0x64; // volant  HEX 64 = 0 centre de -100 à 100 (%)
  xbee_mode_tx=mode;
  Serial2_speed_xbee=speed_xbee;
  //Etablir_liason(xbee_mode_tx);
}


void TruckBee::Envoie(){
  Serial2.write(0x01);
  Serial2.write(memtab[0]);
  Serial2.write(memtab[1]);
  Serial2.write(memtab[2]);
  Serial2.write(memtab[3]);
  Serial2.write('\r');
  #ifdef DEBUG
  Serial.write(0x01);
  Serial.print(memtab[0],HEX);
  Serial.print(memtab[1],HEX);
  Serial.print(memtab[2],HEX);
  Serial.print(memtab[3],HEX);
  Serial.print('\r');
  #endif //DEBUG

}
void TruckBee::Etablir_liason(bool mode){
  #ifdef DEBUG
  Serial.begin(57600);

  Serial.println("lib truckbee , demarrage ...");
  #endif // DEBUG
  Serial2.begin(Serial2_speed_xbee);
  Serial2.print("+++");
  Serial.print("+++");
  char thisByte = 0;
  while (thisByte != '\r') {
    if (Serial2.available() > 0) {
      thisByte = Serial2.read();
    }
  }
  //Serial2.print("ATRE\r");//reset cfg
  //Serial2.print("ATPL4\r");//puissance TX max(0à4)
  //Serial.print("ATDL0\r"); // sent to Serial2 0
  //Serial.print("ATMY1\r"); // this xbee is 1
  //Serial2.print("ATID0B58\r");// reseau xbee adresse == 041986 en HEX
  //Serial2.print("ATWR\r");//enregistre la cfg sur le module
  Serial2.print("ATCN\r");//quitte le mode commande

  #ifdef DEBUG
  Serial.println("Xbee connecté et configuré");
  #endif // DEBUG
}
void TruckBee::Dernier_contact_RF(){// !serialavilable >10 fois ex ou verif etat de la broche Rssi ou led asso du xbee
  if(millis()-Dernier_contact_radio>FAILSAFE || compteur_failsafe>10){
    etat_FAILSAFE();
  }
}
void TruckBee::etat_FAILSAFE(){}
void TruckBee::Recoie(){
  if (Serial2.available() >= 5 ) { // wait for 6 characters
    for (int i=0; i < 6; i++){
      commandes[i] = Serial2.read();
      Serial.print(commandes[i],HEX);
      Serial.print(" ");
    }
    HEX_to_BIN(commandes[2],2);
    Serial.println(" ");
  }
}
  void TruckBee::veil_crois_phares(int A){
    switch (A) {
      case 1:// mode veilleuses
      bitWrite(memtab[1],adrmemtab_veilleuses,true);
      bitWrite(memtab[1],adrmemtab_phares,false);
      bitWrite(memtab[1],adrmemtab_feux_de_crois,false);
      veilleuses=true;
      phares=feux_de_croisements=false;
      break;
      case 2://mode feux de croisement
      bitWrite(memtab[1],adrmemtab_veilleuses,true);
      bitWrite(memtab[1],adrmemtab_feux_de_crois,true);
      bitWrite(memtab[1],adrmemtab_phares,false);
      veilleuses=feux_de_croisements=true;
      phares=false;
      break;
      case 3://mode pleins phares
      bitWrite(memtab[1],adrmemtab_veilleuses,true);
      bitWrite(memtab[1],adrmemtab_feux_de_crois,true);
      bitWrite(memtab[1],adrmemtab_phares,true);
      veilleuses=feux_de_croisements=phares=true;
      break;
      case 0://mode jour (off)
      bitWrite(memtab[1],adrmemtab_veilleuses,false);
      bitWrite(memtab[1],adrmemtab_phares,false);
      bitWrite(memtab[1],adrmemtab_feux_de_crois,false);
      veilleuses=phares=feux_de_croisements=false;
      break;
      default:
      break;
    }
  }
  void TruckBee::clignotants(char B){
    switch(B){
      case 'G'://gauche
      bitWrite(memtab[1],adrmemtab_clignotant_gauche,true);
      bitWrite(memtab[1],adrmemtab_clignotant_droit,false);
      clignotant_droit=false;
      clignotant_gauche=true;
      break;
      case 'D'://droit
      bitWrite(memtab[1],adrmemtab_clignotant_gauche,false);
      bitWrite(memtab[1],adrmemtab_clignotant_droit,true);
      clignotant_droit=true;
      clignotant_gauche=false;
      break;
      case 'W':// warnings
      bitWrite(memtab[1],adrmemtab_clignotant_gauche,true);
      bitWrite(memtab[1],adrmemtab_clignotant_droit,true);
      clignotant_droit=true;
      clignotant_gauche=true;
      break;
      default:
      break;
    }
  }
  void TruckBee::longues_vues(bool A){
    longues_vues_hautes=A;
    if(A){
      bitWrite(memtab[1],adrmemtab_longues_vues_hautes,true);
      longues_vues_hautes=true;
    }else{
      bitWrite(memtab[1],adrmemtab_longues_vues_hautes,false);
      longues_vues_hautes=false;
    }
  }
  void TruckBee::klaxon(int B){
    klaxon_sirene=B;
    switch (B) {
      case 1:
      bitWrite(memtab[1],adrmemtab_klaxon,true);
      break;
      case 0:
      bitWrite(memtab[1],adrmemtab_klaxon,false);
      break;
      default:
      break;
    }
  }
  void TruckBee::direction(int vol){
    volant=constrain(vol,-100,100);
    memtab[3]=map(vol,-100,100,0,0xC8),0,0xC8;
  }
  void TruckBee::marche_moteur(bool M){
    moteur = M;
    if(moteur){
      bitWrite(memtab[1],4,1);
    }else{
      bitWrite(memtab[1],4,0);
    }
  }
  void TruckBee::accel(int acc){
    traction=constrain(acc,-100,100);
    memtab[0]=map(traction,-100,100,0,0xC8);
  }
  void TruckBee::printBits(byte myByte){
    for(byte mask = 0x80; mask; mask >>= 1){
      if(mask  & myByte)
      Serial2.print('1');
      else
      Serial2.print('0');
    }
    Serial2.println();
  }
