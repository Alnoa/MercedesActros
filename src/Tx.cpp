#include <Arduino.h>

//Transmetteur pour le mercedes actros DIY

#include <SoftwareSerial.h>
SoftwareSerial xbee(10,11);
void setup() {
	xbee.begin(57600);
	Serial.begin(115200);
	Serial.println(F("TX Alnoa"));
}

void loop() {
	static int n=0;
	n++;
	delay(1000);
	Serial.println(n);
	xbee.println(n);
}
