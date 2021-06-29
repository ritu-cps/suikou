/**********
 * 冬休み中に種まきする用
 * ---使用品---
 * マイコンボード:Arduino uno
 * センサ:DS18B20(防水仕様)
 * リレーモジュール : 16 Relay Module Low Level Trigger
 * ---ピン配置---
 * リレーモジュール
 *  入力ピン16本(ピン番号とリレー番号は一致) : HIGH入力 = リレーOFF, LOW入力 = リレーON
 * DS18B20
 *  VCC3~5.5V(赤):5V
 *  DATA(黄):
 *  GND(黒):GND
**********/

#include "OneWire.h"
#include "DallasTemperature.h"

#define PIN_KOTATU 2	// コタツ, HIGH = OFF,LOW = ON

#define PIN_ONE_WIRE_BUS 8	// oneWire port
#define SENSER_BIT 9	// 精度の設定bit

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(PIN_ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

const short RoomTmpMAX = 25.0;
const short RoomTmpMIN = 20.0;

/*DeviceAddress tempRight = {0x28,0x8F,0xE3,0x79,0xA2,0x16,0x03,0x62};// this sensor is dead. r.i.p.
DeviceAddress tempCenter = {0x28,0xF1,0xB3,0x79,0xA2,0x00,0x03,0x3A};
DeviceAddress tempLeft = {0x28,0xF9,0xC0,0x79,0xA2,0x00,0x03,0xB8};*/

const short NUMBER_ONEWIRE_SENSOR = 2;
const short PIN_ONEWIRE_CONTROL[] = {4,5,6,7,8,9};
DeviceAddress address_array[NUMBER_ONEWIRE_SENSOR]={{0x28,0xF1,0xB3,0x79,0xA2,0x00,0x03,0x3A},{0x28,0xF9,0xC0,0x79,0xA2,0x00,0x03,0xB8}};//tempCenter,tempLeft
float kionkion[NUMBER_ONEWIRE_SENSOR];

//DeviceAddress address_array[NUMBER_ONEWIRE_SENSOR]={{0x28,0xF1,0xB3,0x79,0xA2,0x00,0x03,0x3A},{0x28,0xF9,0xC0,0x79,0xA2,0x00,0x03,0xB8}};//tempCenter,tempLeft
//address_array[0]={0x28,0xF1,0xB3,0x79,0xA2,0x00,0x03,0x3A};//tempCenter
//address_array[1]={0x28,0xF9,0xC0,0x79,0xA2,0x00,0x03,0xB8};//tempLeft

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
void sensor_test(){//一つずつ1-wireのアドレスを確かめられる
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

	//sensors.getAddress(temp,0);

	pinMode(PIN_KOTATU,OUTPUT);

	//setupOnewire();
}

void countLoop(){
	static int i = 0;
	Serial.println(i);
	i++;
}

// 取得温度
//float temp_array[3] = {0.0,0.0,0.0};

// こたつがついているかどうか
boolean FLAG_KOTATU = false;// true:on, false:off

void loop(){
	delay(3000);
	countLoop();

	readTemperature();

	float temp_ave;//全センサーの平均気温
	float temp_add=0;//平均計算用
	for(int index=0; index < NUMBER_ONEWIRE_SENSOR; index++){
		temp_add += kionkion[index];
	}
	temp_ave = temp_add / NUMBER_ONEWIRE_SENSOR;
	// kotatu関係
	if(!FLAG_KOTATU){
		if(temp_ave<RoomTmpMIN){
			FLAG_KOTATU = true;//on ni suru
		}
	}else{
		if(temp_ave>RoomTmpMAX){
			FLAG_KOTATU = false;//off ni suru
		}
	}
	if(FLAG_KOTATU){
		digitalWrite(PIN_KOTATU,LOW);//AC on
	}else if(!FLAG_KOTATU){
		digitalWrite(PIN_KOTATU,HIGH);//AC off
	}
	//ログ出力
	String log = {String(kionkion[0]) + "\t" + String(kionkion[1]) + "\t" + String(temp_ave) + "\t" + String(FLAG_KOTATU)};
	Serial.println(log);
	//Serial.println(kionkion[0]);
	//Serial.println(kionkion[1]);
	//Serial.println(temp_ave);
	//Serial.println(FLAG_KOTATU);

	/*
	sensors.requestTemperatures();
	temp_array[0] = sensors.getTempC(tempRight);
	temp_array[1] = sensors.getTempC(tempCenter);
	temp_array[2] = sensors.getTempC(tempLeft);

	// kotatu関係
	if(!FLAG_KOTATU){
		if(temp_array[2]<RoomTmpMIN){
			FLAG_KOTATU = true;//on ni suru
		}
	}else{
		if(temp_array[2]>RoomTmpMAX){
			FLAG_KOTATU = false;//off ni suru
		}
	}
	if(FLAG_KOTATU){
		digitalWrite(PIN_KOTATU,LOW);//AC on
	}else if(!FLAG_KOTATU){
		digitalWrite(PIN_KOTATU,HIGH);//AC off
	}

	//watchTemperature();
	Serial.println(temp_array[0]);
	Serial.println(temp_array[1]);
	Serial.println(temp_array[2]);
	Serial.println(FLAG_KOTATU);
	*/
	delay(100);
}