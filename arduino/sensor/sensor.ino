/**********
 * ---使用品---
 * マイコンボード:Arduino uno
 * センサ:DS18B20(防水仕様)
 * ---ピン配置---
 * DS18B20
 *  VCC3~5.5V(赤):5V
 *  DATA(黄):
 *  GND(黒):GND
**********/

#include "OneWire.h"
#include "DallasTemperature.h"

#define PIN_ONE_WIRE_BUS 3	// oneWire port
#define SENSER_BIT 9	// 精度の設定bit

 //0x28 0x50 0x61 0x79 0xA2 0x00 0x03 0x8C
 //0x28 0x98 0xE8 0x79 0x97 0x11 0x03 0x49

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(PIN_ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);


const short NUMBER_ONEWIRE_SENSOR = 2;
const short PIN_ONEWIRE_CONTROL[] = {4,5,6,7,8,9};
DeviceAddress address_array[NUMBER_ONEWIRE_SENSOR];
float kionkion[NUMBER_ONEWIRE_SENSOR];

// get 1-wire sensor addresses in order
// when using multiple 1-wire sensors
void setupOnewire(){
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		pinMode(PIN_ONEWIRE_CONTROL[index], OUTPUT);
		digitalWrite(PIN_ONEWIRE_CONTROL[index], LOW);
	}
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		digitalWrite(PIN_ONEWIRE_CONTROL[index], HIGH);
		oneWire.reset_search();	// 検索状況(アドレスのインデックス)をリセット
		if(oneWire.search(address_array[index], true)){
			//-- do nothing
		}else{
			for(int indexAddress=0; indexAddress<8; indexAddress++){
				address_array[index][indexAddress] = 0;
			}
			Serial.println("error");//---
		}
		digitalWrite(PIN_ONEWIRE_CONTROL[index], LOW);
	}
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		digitalWrite(PIN_ONEWIRE_CONTROL[index], HIGH);
	}
}

// get temperature from all 1-wire sensors
void readTemperature(){
	sensors.requestTemperatures();	// send request all devices
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		//digitalWrite(PIN_ONEWIRE_CONTROL[index], HIGH);
		sensors.requestTemperaturesByAddress(address_array[index]);
		kionkion[index] = sensors.getTempC(address_array[index]);
		//digitalWrite(PIN_ONEWIRE_CONTROL[index], LOW);
	}
}

// send all 1-wire sensor address and temperature
void watchTemperature(){
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		for(int indexAddress=0; indexAddress<8; indexAddress++){
			Serial.print(address_array[index][indexAddress], HEX);
			Serial.print(" ");
		}
		Serial.print(":");
		Serial.println(kionkion[index]);
	}
}

DeviceAddress temp;
uint8_t sensorAddr[8];
void sensor_test(){
	Serial.println("start search ON");
	delay(4000);
	digitalWrite(4, HIGH);	//入力可能

	bool searchMode = true;
	if(oneWire.search(sensorAddr, searchMode)){
		for(int index=0;index<8;index++){
			Serial.print(" ");
			Serial.print(sensorAddr[index], HEX);
		}
		Serial.println();
	}else{
		Serial.println("no");
	}
}

void setup(){
	Serial.begin(115200);
	sensors.setResolution(SENSER_BIT);

	//pinMode(4, OUTPUT);
	//sensor_test();

	/*pinMode(4,OUTPUT);
	pinMode(5,OUTPUT);
	digitalWrite(4,HIGH);
	digitalWrite(5,LOW);
	sensors.getAddress(temp,0);*/

	setupOnewire();
}

void countLoop(){
	static int i = 0;
	Serial.println(i);
	i++;
}

void loop(){
	delay(6000);
	countLoop();
	readTemperature();
	watchTemperature();


	/*for(int i=0;i<8;i++){
    Serial.print(" 0x");
    Serial.print(temp[i],HEX);
  }
	sensors.requestTemperatures();
	Serial.println(sensors.getTempC(temp));
	//Serial.println(sensors.getTempC(sensorAddr));*/

	//sensors.requestTemperatures();              // 温度取得要求
  //Serial.println(sensors.getTempCByIndex(0)); //温度の取得&シリアル送信
}