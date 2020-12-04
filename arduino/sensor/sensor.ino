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

OneWire oneWire(PIN_ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(){
	Serial.begin(9600);
	sensors.setResolution(SENSER_BIT);
}

void loop(){
	sensors.requestTemperatures();              // 温度取得要求
  Serial.println(sensors.getTempCByIndex(0)); //温度の取得&シリアル送信
}