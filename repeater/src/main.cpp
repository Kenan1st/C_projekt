#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

#define CNS 19
#define CE 18
#define JOYSTICKX 4
#define JOYSTICKY 2
#define BUTTON 15

RF24 radio(CNS,CE);

const byte address[6] = "00001";

bool joystickcontrol = 1;
bool lastButtonState = 0;

/*typedef struct{
	int angle;
	int thrust;
} SENSOR_DATA;
*/
void initMpu(){
	mpu.begin();
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
}

/*void initRadio(){
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
}*/

void setup() {
	Serial.begin(115200);
	SPI.begin(21, 22, 23);
	initMpu();
	//initRadio();
	pinMode(BUTTON, INPUT_PULLUP);
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();
}

void loop(){
	delay(5);

	bool currentButtonState = analogRead(BUTTON) < 100;

	if (currentButtonState && !lastButtonState){
		joystickcontrol = !joystickcontrol;
	}

	lastButtonState = currentButtonState;


	if(joystickcontrol){
		int potValueX = analogRead(JOYSTICKX);
		int potValueY = analogRead(JOYSTICKY);
		Serial.printf("X: %d",potValueX);
		Serial.printf("Y: %d",potValueY);
		//int angle = map(potValueX,0,1023,0,360);
		int thrust = map(potValueY,0,1023,0,512);

		//SENSOR_DATA sendData= {angle,thrust};

		radio.write(&thrust,sizeof(thrust));
	}/*
	else{
		sensors_event_t a, g, temp;
    		mpu.getEvent(&a, &g, &temp);
		int accValue = a.acceleration.z;
		int angValue = (g.gyro.x+g.gyro.y)/2;

		int angle = map((int)angValue,-250,250,0,360);
		int thrust = map((int)accValue,0,8,0,360);

		SENSOR_DATA sendData= {angle,thrust};

		radio.write(&sendData,sizeof(sendData));

	}*/
}
