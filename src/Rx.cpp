/*http://busyducks.com/ascii-art-arduinos*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define DEBUG
/**
 * Raccourcis
 */
#define Pin_neopixels 6
#define NUMPIXELS 9

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

/**
 * Librairies
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, Pin_neopixels, NEO_GRB + NEO_KHZ800);
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//#include <SoftwareSerial.h>
#include <truckbee.h>
TruckBee TruckBeeRX;
/**
 * variables
 */
unsigned long dernier_click_cligno=0;
unsigned long frequence_clignotant=550;
bool etat_clign_demi_hertz=false;
	int lasttt=0;//temp

void setup() {
	lcd.init();
	lcd.clear();
	lcd.backlight();
	lcd.setCursor(0,0);
	lcd.print("Hello world");
	#ifdef DEBUG
	Serial.begin(115200);
	Serial.println(F("RX Alnoa"));
	#endif //DEBUG

	pixels.begin();
	//pixels.setPixelColor(1,pixels.Color(46, 8, 18));
	pixels.show();
	pwm.begin();
	pwm.setPWMFreq(1000);
	//TruckBeeRX.Etablir_liason();
}
/**
 * [routine_demi_hertz description]
 * @return [etat clignotants]
 * une routine de clignotement à 1/2hertz (lent)
 */
bool routine_demi_hertz(){
	unsigned long millis_actuel= millis();
	if(millis_actuel-dernier_click_cligno>frequence_clignotant){
		dernier_click_cligno=millis_actuel;
		etat_clign_demi_hertz= !etat_clign_demi_hertz;
	}
	return etat_clign_demi_hertz;
}

/**
 * [trait_lumieres description]
 * Recupere les etat des variables d'eclairage du modele pour
 * les affecte au sorties physiques
 */
void trait_lumieres(){
	if(TruckBeeRX.veilleuses){pixels.setPixelColor(Pin_veilleuses,pixels.Color(10, 10, 10));}else{pixels.setPixelColor(Pin_veilleuses,pixels.Color(0, 0, 0));}
	if(TruckBeeRX.feux_de_croisements){pixels.setPixelColor(Pin_feux_de_croisements,pixels.Color(50,50,50));}else{pixels.setPixelColor(Pin_feux_de_croisements,pixels.Color(0, 0, 0));}
	if(TruckBeeRX.phares){pixels.setPixelColor(Pin_phares,pixels.Color(200,200,200));}else{pixels.setPixelColor(Pin_phares,pixels.Color(0, 0, 0));}
	if(TruckBeeRX.clignotant_droit){if(routine_demi_hertz()){pixels.setPixelColor(Pin_cligno_droit,pixels.Color(255, 176, 0));}else{pixels.setPixelColor(Pin_cligno_droit,pixels.Color(0,0,0));}}else{pixels.setPixelColor(Pin_cligno_droit,pixels.Color(0,0,0));}

	if(TruckBeeRX.veilleuses){pwm.setPWM(Pin_veilleuses,0,4096);}else{pwm.setPWM(Pin_veilleuses,0,0);}
	if(TruckBeeRX.feux_de_croisements){pwm.setPWM(Pin_feux_de_croisements,0,4096);}else{pwm.setPWM(Pin_feux_de_croisements,0,0);}
	if(TruckBeeRX.phares){pwm.setPWM(Pin_phares,0,4096);}else{pwm.setPWM(Pin_phares,0,0);}
	if(TruckBeeRX.clignotant_droit){if(routine_demi_hertz()){pwm.setPWM(Pin_cligno_droit,0,4096);}else{pwm.setPWM(Pin_cligno_droit,0,0);}}else{pwm.setPWM(Pin_cligno_droit,0,0);}
	if(TruckBeeRX.clignotant_gauche){if(routine_demi_hertz()){pwm.setPWM(Pin_cligno_gauche,0,4096);}else{pwm.setPWM(Pin_cligno_gauche,0,0);}}else{pwm.setPWM(Pin_cligno_gauche,0,0);}
	if(TruckBeeRX.longues_vues_hautes){pwm.setPWM(Pin_longues_vues_hautes,0,4096);}else{pwm.setPWM(Pin_longues_vues_hautes,0,0);}
	if(TruckBeeRX.antibrouillards){pwm.setPWM(Pin_antibrouillards,0,4096);}else{pwm.setPWM(Pin_antibrouillards,0,0);}
	pixels.show();
}

void loop(){
//TruckBeeRX.clignotants('D');
//TruckBeeRX.veil_crois_phares(lasttt);
//trait_lumieres();
delay(500);
//TruckBeeRX.Envoie();
lcd.setCursor(0,1);
lcd.print(TruckBeeRX.memtab[0],HEX);
lcd.setCursor(0,2);
lcd.print(TruckBeeRX.memtab[1],BIN);
lcd.setCursor(0,3);
lcd.print(lasttt);
TruckBeeRX.Recoie();
//delay(100);

lasttt++;
if(lasttt==4){
	lasttt=0;
	}
	lcd.clear();
}
