"""これは、mqttのメッセージを受信,送信クラスがある\n
　使用方法
    1.topic
    2.sub：引数で受信データをもらえるから自分で作ったメソッドで処理
    　pub：メッセージ内容を返すメソッド
    3.ip\n
指定すれば最低限動くはず"""

import paho.mqtt.client as mqtt     # MQTTのライブラリをインポート
from time import sleep              # 3秒間の停止のために使う

# ブローカーに接続できたときの処理


class Mqtt_code_sub():
    """ topic, 処理メソッド, ipは最低限与えてください"""

    def __init__(self, topic, syori, ip, port=1883, mqttKeepAlve=1000):
        self.syori = syori
        self.topic = topic
        self.ip = ip
        self.port = port
        self.mkal = mqttKeepAlve

    def on_connect(self, client, userdata, flag, rc):
        print("Connected with result code " + str(rc))  # 接続できた旨表示
        client.subscribe(self.topic)  # subするトピックを設定

    # ブローカーが切断したときの処理

    def on_disconnect(self, client, userdata, flag, rc):
        if rc != 0:
            print("Unexpected disconnection.")

    # メッセージが届いたときの処理

    def on_message(self, client, userdata, msg):
        # msg.topicにトピック名が，msg.payloadに届いたデータ本体が入っている
        # print("Received message '" + str(msg.payload) +
        #       "' on topic '" + msg.topic + "' with QoS " + str(msg.qos))
        
        # data = json.loads(msg.payload.decode("utf-8"))
        self.syori(msg.payload.decode("utf-8"))
  
    def Mqttsub(self):
        # MQTTの接続設定
        client = mqtt.Client()                 # クラスのインスタンス(実体)の作成
        client.on_connect = self.on_connect         # 接続時のコールバック関数を登録
        client.on_disconnect = self.on_disconnect   # 切断時のコールバックを登録
        client.on_message = self.on_message  # メッセージ到着時のコールバック
        #                IP, PORT, MQTT_KEEP_ALIVE
        client.connect(self.ip, self.port, self.mkal)  # 接続先は自分自身

        client.loop_forever()  # 永久ループして待ち続ける


class mqtt_publish():
    """ topic, メッセージメソッド, ipは最低限与えてください"""
    def __init__(self, topic, message_method, ip, port=1883, mqtt_keep_alive=60):
        self.ip = ip
        self.port = port
        self.mkal = mqtt_keep_alive
        self.message_method = message_method
        self.topic = topic

    # ブローカーに接続できたときの処理
    def on_connect(self, client, userdata, flag, rc):
        print("Connected with result code " + str(rc))
    # ブローカーが切断したときの処理

    def on_disconnect(self, client, userdata, flag, rc):
        if rc != 0:
            print("Unexpected disconnection.")
    # publishが完了したときの処理

    def on_publish(self, client, userdata, mid):
        print("publish: {0}".format(mid))
    # メイン関数   この関数は末尾のif文から呼び出される

    def Mqttpub(self):
        client = mqtt.Client()                 # クラスのインスタンス(実体)の作成
        client.on_connect = self.on_connect         # 接続時のコールバック関数を登録
        client.on_disconnect = self.on_disconnect   # 切断時のコールバックを登録
        client.on_publish = self.on_publish         # メッセージ送信時のコールバック

        client.connect(self.ip, self.port, self.mkal)  # 接続先は自分自身

        # 通信処理スタート
        client.loop_start()    # subはloop_forever()だが，pubはloop_start()で起動だけさせる

        # 永久に繰り返す
        while True:
            client.publish(self.topic,
                           self.message_method())    # トピック名とメッセージを決めて送信
            sleep(3)   # 3秒待つ


# //////////テストで使用しためそっど////////////


def testsyori(data):
    print(data)


def message_test():
    return "これは：{}".format("test")
# ///////////////////////////////////////////
