#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

#define CNS 1
#define CE 2
#define JOYSTICK 3
#define BUTTON 4

RF24 radio(CNS,CE);

const byte address[6] = "00001";

bool joystickcontrol = 1;
bool lastButtonState = 0;

struct Controlpack{
	int angle;
	int thrust;
};

void initMpu(){
	mpu.begin();
	mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
}

void initRadio(){
	radio.begin();
	radio.openWritingPipe(address);
	radio.setPALevel(RF24_PA_MIN);
	radio.stopListening();
}

void setup() {
	Serial.begin(115200);
	initMpu();
	initRadio();
	
}

void loop(){
	delay(5);

	bool currentButtonState = analogRead(BUTTON) < 100;

	if (currentButtonState && !lastButtonState){
		joystickcontrol = !joystickcontrol;
	}

	lastButtonState = currentButtonState;


	if(joystickcontrol){
		int potValue = analogRead(JOYSTICK);
		int angle = map(potValue,0,1023,0,360);
		int thrust = map(potValue,0,1023,0,512);

		Controlpack sendpack= {angle,thrust};

		radio.write(&sendpack,sizeof(sendpack));
	}
	else{
		sensors_event_t a, g, temp;
    		mpu.getEvent(&a, &g, &temp);
		float accValue = a.acceleration.z;
		float angValue = (g.gyro.x+g.gyro.y)/2;

		int angle = map((int)angValue,-250,250,0,360);
		int thrust = map((int)accValue,0,8,0,360);

		Controlpack sendpack= {angle,thrust};

		radio.write(&sendpack,sizeof(sendpack));

	}
}
