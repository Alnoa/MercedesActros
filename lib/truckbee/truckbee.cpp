#include "truckbee.h"
#if (ARDUINO < 100)
    #include "WProgram.h"
  #else
    #include "Arduino.h"
#endif
#include <arduino.h>
#include "SoftwareSerial.h"

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




TruckBee::TruckBee(){
	memtab[0] = 0xFF;
	memtab[1] = 0b00000000;
	memtab[2] = 0b00000000;
  SoftwareSerial xbee(2,3); // RX TX
	xbee.begin(57600);
}


void TruckBee::Envoie(){
	xbee.print(0x01);
	xbee.print(0x20);
	xbee.print(memtab[0]);
	xbee.print(memtab[1]);
	xbee.print(memtab[2]);
	xbee.print(0x20);
	xbee.print(0x04);
	#ifdef DEBUG
	Serial.write(0x01);
	Serial.write(0x20);
	Serial.print(memtab[0],HEX);
	Serial.print(memtab[1],BIN);
	Serial.print(memtab[2],BIN);
	Serial.write(0x20);
	Serial.write(0x04);
	Serial.println();
	#endif //DEBUG

}
void TruckBee::Etablir_liason(){
  while ( xbee.available()<=0){
    xbee.print("G");
  }
}
void TruckBee::Dernier_contact_RF(){// !serialavilable >10 fois ex
  if(millis()-Dernier_contact_radio>FAILSAFE || compteur_failsafe>10){
    etat_FAILSAFE();
  }
}
void TruckBee::etat_FAILSAFE(){
}
void TruckBee::Recoie(){
	if(xbee.available()){
		Serial.println(xbee.read());
    	/*while (xbee.read() !=-1); {
    		compteur_failsafe=0;
    	}else{
			compteur_failsafe++;
		}*/
	}else{Serial.println("Rien");}
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
	if(A){
		bitWrite(memtab[1],adrmemtab_longues_vues_hautes,true);
		longues_vues_hautes=true;
	}else{
		bitWrite(memtab[1],adrmemtab_longues_vues_hautes,false);
		longues_vues_hautes=false;
	}
}
void TruckBee::klaxon(int B){
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
void TruckBee::marche_moteur(){
	moteur = !moteur;
	if(moteur){
		bitWrite(memtab[1],4,1);
	}else{
		bitWrite(memtab[1],4,0);
	}
}
void TruckBee::printBits(byte myByte){
	for(byte mask = 0x80; mask; mask >>= 1){
		if(mask  & myByte)
			Serial.print('1');
		else
			Serial.print('0');
	}
	Serial.println();
}
