
#ifndef truckbee_h
#define truckbee_h

#include <inttypes.h>
#include <arduino.h>
//
class TruckBee{
public:
	//constructeur
	TruckBee(int speed_xbee,bool mode);
	//modificateurs
	void Etablir_liason(bool mode);
	void Dernier_contact_RF();
	void etat_FAILSAFE();
	void Envoie();
	void Recoie();
	void veil_crois_phares(int A);
	void longues_vues(bool A);
	void klaxon(int B);
	void marche_moteur(bool M);
	void accel(int acc);
	void direction(int vol);
	void clignotants(char B);

	//accesseurs
	//Péripheriques
	bool remorque(){return remorque_etat;}
	uint8_t batterie(){return niveau_batterie;}
	//eclairages
	bool getVeilleuse(){return veilleuses;}
	bool getPhares(){return phares;}
	bool getFeux_de_croisements(){return feux_de_croisements;}
	bool getGyrophares(){return gyrophares;}
	bool getLongues_vues_hautes(){return longues_vues_hautes;}
	bool getAntibrouillards(){return antibrouillards;}
	bool getKlaxon(){return klaxon_sirene;}
	bool getClignotant_droit(){return clignotant_droit;}
	bool getClignotant_gauche(){return clignotant_gauche;}
	//Motorisation
	int8_t getTraction(){return traction;}
	int8_t getDirection(){return volant;}
	bool getContact_moteur(){return moteur;}
	bool getMarche_avant_moteur(){return marche_avant_moteur;}
	uint8_t getRapport_de_BV(){return rapport_boite_de_vitesse;}

	void printBits(byte myByte);


protected:
	unsigned long Serial2_speed_xbee;
	bool xbee_mode_tx;
	unsigned long Dernier_contact_radio=0;
	unsigned long FAILSAFE =1000;
	unsigned int compteur_failsafe=0;
private:
	byte memtab[6];
	/**
	* Périphériques
	*/
	bool remorque_etat;
	uint8_t niveau_batterie;
	/**
	* Eclairage
	*/
	bool veilleuses;
	bool gyrophares;
	bool phares;
	bool feux_de_croisements;
	bool clignotant_droit;
	bool clignotant_gauche;
	bool longues_vues_hautes;
	bool antibrouillards;
	bool klaxon_sirene;
	//beep de marche arriere 2900hz tone(PIN, FREQUENCE hz, DURÉE ms (opt) );  arrêter fonction noTone(PIN)

	/**
	* Motorisation
	*/
	int8_t	traction = 0;
	int8_t volant = 0;
	bool moteur;
	bool marche_avant_moteur;
	uint8_t rapport_boite_de_vitesse;

	/**
	 * variables de methodes de classes
	 */
	byte commandes[10];

	void HEX_to_BIN(byte monByte,uint8_t casier_RAM){
    unsigned int compteurHEx=7;
    for(byte mask = 0x80; mask; mask >>= 1){
      if(mask  & monByte){
        //true
        switch (compteurHEx) {
          case 7:{}break;
          case 6:{}break;
          case 5:{}break;
          case 4:{}break;
          case 3:{}break;
          case 2:{ phares=true;}break;
          case 1:{ feux_de_croisements=true;}break;
          case 0:{ veilleuses=true;}break;
          default:
          break;
        }
      }else{
        //false
        switch (compteurHEx) {
        case 7:{}break;
        case 6:{}break;
        case 5:{}break;
        case 4:{}break;
        case 3:{}break;
        case 2:{ phares=false;}break;
        case 1:{ feux_de_croisements=false;}break;
        case 0:{ veilleuses=false;}break;
        default:
        break;
      }
      compteurHEx--;
    }
  }
}

};
#endif
