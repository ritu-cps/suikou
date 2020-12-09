# 水耕栽培管理

水耕栽培時の自動化を目的としています。

## Arduino の追加ライブラリについて

下記リンクのライブラリを入れてくれ
[Arduino 用ライブラリ LINK](https://drive.google.com/drive/folders/1qeW-CQY6QfRc4KwfLRRFvBdl0Zmwngdo?usp=sharing)

### ライブラリの入れ方

ライブラリのインストールについては次を参照してください：[URL](http://www.arduino.cc/en/Guide/Libraries)

#### 要約

ここにぶち込め(mac なら)

```cmd
/Users/ユーザー名/Documents/Arduino/libraries
```

## raspberryPI

リレー制御、センサーの管理統括、WEBAPI での操作などを行います。

### プログラムファイル

- loadenv
  WEBAPI、センサー値などの変数を保存しておく

- Serial_API
  リレー制御ように作成。WEB 上で切り替えることができる

- Suikou_Serial
  リレーに対して信号を送り制御する。
