import serial

"""
水耕栽培のリレー制御のメッセージを送信することです。
プロンプトは、シリアル通信です。
"""


def send_message(mes):
    """
    messageを送信します。
    """
    print(mes)
    ser = serial.Serial('/dev/ttyS0', 115200)
    print("send")
    
    ser.write(mes.encode())
    ser.close()


def get_message(num):
    """
    messageを指定した分読むことができます。
    """
    print("start")
    ser = serial.Serial('/dev/ttyS0', 115200)
    
    ser.write("mode\n".encode())
    message = ser.read(num)
    
    ser.close()
    print(message)
    return message


if __name__ == "__main__":

    send_message("1:1")
