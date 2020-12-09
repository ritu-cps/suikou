import sys
import Suikou_Serial

"""
コマンドラインでの実行で動作する
例- python3 Control 2:1
"""
args = sys.argv
print(args)

Suikou_Serial.send_message(args[1])
