from flask import Flask, request
from flask_cors import CORS

import Suikou_Serial
import loadenv
app = Flask(__name__)
CORS(app)

# webapiのパスワード
password = loadenv.WEBPASS


@app.route('/')
def suikou():
    title = "水耕栽培の管理システム"
    return title


def serial_send(cnumber, switch):
    if cnumber < 5 and cnumber != 0:
        Suikou_Serial.send_message("{}:{}".format(cnumber, switch))
    else:
        pass


@app.route('/Control', methods=['POST'])
def good():
    """
    リレー制御
    リクエスト
    Cnumber : リレー番号
    ONorOFF : どう切り替えるか
    """
    result = "result code 200"
    if request.method == 'POST':
        if request.form['pass'] == password:
            control_number = request.form['Cnumbar']
            switch_number = 1 if request.form['ONorOFF'] == "ON" else 0

            # リレーにリクエスト
            serial_send(control_number, switch_number)
        else:
            result = "result error code 403"

    else:
        result = "result error code 400"

    return result


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True, threaded=True)
