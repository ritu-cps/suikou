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
const short NUMBER_RELAY = 2;	// 使用リレー数
const short PIN_RELAY_ARRAY[NUMBER_RELAY] = {2,3};	// HIGH = OFF,LOW = ON, {ポンプ, ヒーター}
boolean FLAG_RELAY[NUMBER_RELAY] = {false,false};

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

boolean receiveSerial(int *buf_serial, short size_read_buf){	// return true == ok, return false == error
	/* size_read_buf : 今回読みたい文字数 */
	/* buf_serial : MAX 64byte:Serial buffer size in arduino uno */
	short index_serial = 0;	// number of buf_serial(MAX:64)
	delay(5);	// wait time to receive all serial text(64[byte]/115200[bps])
	while(Serial.available() > 0){
		buf_serial[index_serial] = Serial.read();
		index_serial++;
		if(index_serial >= size_read_buf){
			while(Serial.available() > 0){	// read buffer clear
				Serial.read();
			}
			return true;	// 必要文字数取得完了
		}
	}
	return false;	// 必要文字数足りない
}
boolean readCommand(int *command){	// return true == ok, return false == error
/*	const byte command_number[] = {65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80};	// {A~P} (if use lowercase a~p, ==98~108)
	if(command[1] == 58){	// 58 = ":"
		for(int index=0; index < NUMBER_RELAY; index++){
			if(command[0] == command_number[index]){
				if(command[2] == 49){	// turn on (49='1')
					FLAG_RELAY[command[0]-65] = true;
					return 0;
				}
				else if(command[2] == 48){	// turn off (48='0')
					FLAG_RELAY[command[0]-65] = false;
					return 0;
				}
			}
		}
	}
	return 1;*/

	if(command[1] == 58){	// 58 = ":"
		if((command[0] >= 65)&&(command[0] < 65 + NUMBER_RELAY)){	// 65='A', MAX 80='P' (, if use lowercase a~p, ==98~108)
			if(command[2] == 49){	// turn on (49='1')
				FLAG_RELAY[command[0]-65] = true;
				return true;
			}else if(command[2] == 48){	// turn off (48='0')
				FLAG_RELAY[command[0]-65] = false;
				return true;
			}
		}
	}
	return false;
}
void outputRelay(){
	for(int index=0; index < NUMBER_RELAY; index++){
		if(FLAG_RELAY[index] == true){	// turn on
			digitalWrite(PIN_RELAY_ARRAY[index],LOW);
		}else if(FLAG_RELAY[index] == false){	// turn off
			digitalWrite(PIN_RELAY_ARRAY[index],HIGH);
		}
	}
	return;
}

void setup(){
	/*pinMode(PIN_RELAY_1, OUTPUT);
	pinMode(PIN_RELAY_2, OUTPUT);
	digitalWrite(PIN_RELAY_1, HIGH);
	digitalWrite(PIN_RELAY_2, HIGH);*/

	for(int index=0; index < NUMBER_RELAY; index++){
		pinMode(PIN_RELAY_ARRAY[index], OUTPUT);
	}
	outputRelay();

	Serial.begin(115200);
}
void loop(){
	if(Serial.available() > 0){
		const short size_receive_buffer = 3;
		int buffer_receive_serial[size_receive_buffer];
		if(receiveSerial(buffer_receive_serial, size_receive_buffer) == true){
			if(readCommand(buffer_receive_serial) == true){
				outputRelay();
				for(int index = 0; index < size_receive_buffer; index++)
					Serial.print((char)buffer_receive_serial[index]);
			}else{
				Serial.println("command error");
			}
		}else{
			Serial.println("receive error");
		}
	}

/*	int bufbuf[50];
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
	}*/
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
		readCommand(bufbuf);
	}*/
}
