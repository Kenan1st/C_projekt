#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdio.h>
#include <ESP32_Servo.h>

/*#define MS1 7
#define M1V 8
#define M1Z 9
#define M2V 10
#define M2Z 11

#define MS2 12
#define M3V 13
#define M3Z 14
#define M4V 15
#define M4Z 16*/

#define CSN 18
#define CE 19

RF24 radio(CSN,CE);

/*#define SPIN1 21
#define SPIN2 22

Servo servo1;
Servo servo2;*/

/*typedef struct{
	int angle;
	int thrust;
}SENSOR_DATA;
*/
unsigned int address = 12345;

/*void setupMotor(){
	pinMode(MS1,OUTPUT);
	pinMode(MS2,OUTPUT);
	pinMode(M1V,OUTPUT);
	pinMode(M1Z,OUTPUT);
	pinMode(M2V,OUTPUT);
	pinMode(M2Z,OUTPUT);
	pinMode(M3V,OUTPUT);
	pinMode(M3Z,OUTPUT);
	pinMode(M4V,OUTPUT);
	pinMode(M4Z,OUTPUT);

}*/

void initRadio(){

	radio.begin();
	radio.setChannel(10);
	radio.setPALevel(RF24_PA_MAX);
	radio.setDataRate(RF24_250KBPS);
	radio.openReadingPipe(1,address);
	radio.startListening();
}

/*void driveFor(){
	digitalWrite(M1V,HIGH);		// Wenn V High ist fährt man vorwärts (Z muss LOW sein)
	digitalWrite(M2Z,LOW);		// Wenn Z High ist fährt man rückwärts (V muss LOW sein)
	digitalWrite(M2V,HIGH);
	digitalWrite(M2Z,LOW);
	digitalWrite(M3V,HIGH);
	digitalWrite(M3Z,LOW);
	digitalWrite(M4V,HIGH);
	digitalWrite(M4Z,LOW);

}*/

/*void driveBack(){
	digitalWrite(M1V,LOW);		// Die Motoren haben eine mayimal geschwindigkeitsstufe von 255
	digitalWrite(M1Z,HIGH);
	digitalWrite(M2V,LOW);
	digitalWrite(M2Z,HIGH);
	digitalWrite(M3V,LOW);
	digitalWrite(M3Z,HIGH);
	digitalWrite(M4V,LOW);
	digitalWrite(M4Z,HIGH);
}*/

void setup(){
	Serial.begin(115200);
	delay(1000);
	//setupMotor();

	//servo1.attach(SPIN1);
	//servo2.attach(SPIN2);

	initRadio();
}

void loop(){
	delay(5);
		while(radio.available()){
			int received;

			radio.read(&received,sizeof(received));

			Serial.printf("thrust: %d \n"/*| angle: %d \n",received.thrust,received.angle);*/,received);

			/*analogWrite(MS1,thrust);
			analogWrite(MS2,thrust);

			if(thrust<127){
				driveBack();
			}
			else{
				driveFor();
			}

			servo1.write(constrain(angle,0,180));
			servo2.write(constrain(360 - angle, 0, 180));*/
		}
}
