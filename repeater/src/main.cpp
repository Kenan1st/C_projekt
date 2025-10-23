#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdio.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;

#define CNS 5
#define CE 4
#define JOYSTICKX 13
#define JOYSTICKY 12
#define BUTTON 15

RF24 radio(CNS,CE);

unsigned int address = 12345;

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

void initRadio(){
	radio.begin();
	radio.setChannel(10);
	radio.setPALevel(RF24_PA_HIGH);
	radio.setDataRate(RF24_250KBPS);
	radio.openWritingPipe(address);
}

void setup() {
	Serial.begin(115200);
	initRadio();
	initMpu();
	pinMode(BUTTON, INPUT_PULLUP);
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
		Serial.printf("Y: %d\n",potValueY);
		//int angle = potValueX-1905;
		//int thrust = (potValueY-1450);

		//SENSOR_DATA sendData= {angle,thrust};

		radio.write(&potValueX,sizeof(potValueX));
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
