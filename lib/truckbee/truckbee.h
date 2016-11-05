
#ifndef truckbee_h
#define truckbee_h

#include <inttypes.h>
#include <arduino.h>
#include <SoftwareSerial.h>
//
class TruckBee{
public:
	//constructeur
	TruckBee();
	void Etablir_liason();
	void Dernier_contact_RF();
	void etat_FAILSAFE();
	void Envoie();
	void Recoie();
	void veil_crois_phares(int A);
	void longues_vues(bool A);
	void klaxon(int B);
	void marche_moteur();
	void accel();
	void direction();
	void clignotants(char B);
	bool remorque(){return remorque_etat;}
	unsigned int batterie(){return niveau_batterie;}
	byte memtab[6];
	void printBits(byte myByte);

	bool remorque_etat;
	unsigned int niveau_batterie;
	int	traction = 0;
	int volant = 0;
	bool veilleuses;
	bool gyrophares;
	bool phares;
	bool feux_de_croisements;
	bool clignotant_droit;
	bool clignotant_gauche;
	bool longues_vues_hautes;
	bool antibrouillards;
	bool sirene;
	bool moteur;

protected:
	unsigned long Dernier_contact_radio=0;
	unsigned long FAILSAFE =1000;
	unsigned int compteur_failsafe=0;
private:

};
#endif
