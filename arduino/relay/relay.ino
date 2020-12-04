/**********
 * ---使用品---
 * マイコンボード : Arduino uno
 * リレーモジュール : 16 Relay Module Low Level Trigger
 * ---ピン配置---
 * リレーモジュール
 *  入力ピン16本(ピン番号とリレー番号は一致) : HIGH入力 = リレーOFF, LOW入力 = リレーON
**********/

#define PIN_RELAY_1 2	// ポンプ, HIGH = OFF,LOW = ON
#define PIN_RELAY_2 3	// ヒーター, HIGH = OFF,LOW = ON
/*#define PIN_RELAY_3 4
#define PIN_RELAY_4 5
#define PIN_RELAY_5 6
#define PIN_RELAY_6 7
#define PIN_RELAY_7 8
#define PIN_RELAY_8 9
#define PIN_RELAY_9
#define PIN_RELAY_10
#define PIN_RELAY_11
#define PIN_RELAY_12
#define PIN_RELAY_13
#define PIN_RELAY_14
#define PIN_RELAY_15
#define PIN_RELAY_16*/

int kaiseki(int *command){
	if(command[1] == 58){ // 48 = ":"
		int pin_number = 0;
		if(command[0] == 49)  // 49 = "1"
			pin_number = 2;
		else if(command[0] == 50)
			pin_number = 3;
		else
			return 0;
		
		if(command[2] == 49)
			digitalWrite(pin_number, LOW);
		else if(command[2] == 48)
			digitalWrite(pin_number, HIGH);
		else
			return 0;
	}
	else
		return 0;
	return 1;
}

void setup(){
	pinMode(PIN_RELAY_1, OUTPUT);
	pinMode(PIN_RELAY_2, OUTPUT);

	digitalWrite(PIN_RELAY_1, HIGH);
	digitalWrite(PIN_RELAY_2, HIGH);

	Serial.begin(115200);
}
void loop(){
	int bufbuf[50];
	int index = 0;
	int buf_length = 0;
	while(Serial.available() > 0){
		delay(100);
		bufbuf[index] = Serial.read();
		index += 1;
		if(index >= 3){
			while(Serial.available()>0){
				bufbuf[index] = Serial.read();
				index++;
			}
			int num_kaiseki = kaiseki(bufbuf);
			if(num_kaiseki == 0){
				Serial.print("read error");
			}
			else if(num_kaiseki == 1){
				Serial.print("ok");
			}
			for(index = 0; index <=3; index++)
				Serial.print((char)bufbuf[index]);
			index = 0;
			break;
		}
	}
  /*if(Serial.available()>0){
    delay(100);
    bufbuf = Serial.read();
    Serial.print(bufbuf);
    if(bufbuf == 49)
      Serial.println("ok");
    else
      Serial.println("no");
  }*/
	//String bufstr2;
	/*if(Serial.available() > 0){
		bufbuf = Serial.read();
		Serial.readString(bufstr);
		kaiseki(bufbuf);
	}*/
}
