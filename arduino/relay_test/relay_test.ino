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

//#define PIN_DS18B20_DATA 3

#define ONE_WIRE_BUS 3 // データ(黄)で使用するポート番号
#define SENSER_BIT    9      // 精度の設定bit

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(){
	//pinMode(PIN_DS18B20_DATA, INPUT);
	Serial.begin(9600);
	sensors.setResolution(SENSER_BIT);
}

void loop(){
	sensors.requestTemperatures();              // 温度取得要求
  Serial.println(sensors.getTempCByIndex(0)); //温度の取得&シリアル送信
}