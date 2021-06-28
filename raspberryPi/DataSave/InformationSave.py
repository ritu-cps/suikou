from Component import plantMqtt
from Component import database
import json
import datetime
import csv
import os


class plantSave():

    def __init__(self):
        self.saveBox = {}
        self.total = 1
        self.timer = datetime.datetime.now() + datetime.timedelta(minutes=1)

    def infoSaveTocsv(self):
        try:
            with open("{}.csv".format(datetime.date.today()), newline='') as f:
                r = csv.DictReader(f)
                data = [line for line in r]
            with open("{}.csv".format(datetime.date.today()), 'w', newline='') as f:
                w = csv.DictWriter(f, self.saveBox.keys())
                w.writeheader()
                data.append(self.saveBox)
                newData = data
                w.writerows(newData)

        except FileNotFoundError:
            with open("{}.csv".format(datetime.date.today()), 'w') as w:
                writer = csv.DictWriter(w, self.saveBox.keys())
                writer.writeheader()
                writer.writerow(self.saveBox)
            self.databaseSave("{}.csv".format(datetime.date.today()))

    def databaseSave(self, fileName):
        database.db_request(
            'INSERT INTO plantData (id,csvpath,appname) VALUES (1,"{}/{}","{}")'.format(os.path.dirname(__file__), fileName, "testApp"))

    def mqttSaveProcess(self, mqttData):

        # json形式
        jsonData = json.loads(mqttData)

        for k in jsonData.keys():
            # 逐次平均を求める
            if k in self.saveBox.keys():
                if jsonData[k] is None:
                    pass
                else:
                    self.saveBox[k] = round(self.saveBox[k] +
                                            (float(jsonData[k]) - self.saveBox[k]) / self.total, 3)
            else:
                self.saveBox[k] = round(float(jsonData[k]), 3)

        self.total += 1

        if datetime.datetime.now().strftime('%M') == self.timer.strftime('%M'):
            self.saveBox["Time"] = datetime.datetime.now()
            self.infoSaveTocsv()
            self.saveBox = {}
            self.total = 1
            self.timer = datetime.datetime.now() + datetime.timedelta(minutes=1)
            print(self.timer.strftime('%M'))


save = plantSave()
test = plantMqtt.Mqtt_code_sub("DHT", save.mqttSaveProcess, "192.168.10.108")
test.Mqttsub()
