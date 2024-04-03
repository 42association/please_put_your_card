# please_put_your_card

## 概要
M5StickCPlus2でNFCカードのUIDをサーバーに送信するためのQRコードを表示します。

## ハードウェア要件
- [M5StickCPlus2](https://www.switch-science.com/products/9350)
- [RFIDカードリーダー](https://www.switch-science.com/products/8301)

## ソフトウェア要件
- [Arduino IDE](https://www.arduino.cc/en/software)
- [VScode](https://code.visualstudio.com/)
- [platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## インストール
1. **Arduino IDEセットアップ：** Arduino IDEをインストール

2. **VScodeをセットアップ:** VScodeをインストールします。

3. **PlatformIOセットアップ：** VScodeの拡張機能であるPlatformIO IDEをインストール

## 設定
- **WiFiセットアップ：** `secrets.h`ファイルにWiFiの認証情報を記入
- **サーバーセットアップ：** `secrets.h`ファイルにサーバー設定を記入
```cpp
const char* ssid = "";
const char* password = "";
const char* serverName = "";
```

## 使用方法
1. `functions.ino`および`please_put_your_card.ino`のスケッチをM5StickCPlus2にアップロードします。
2. デバイスはこれでカードを読み取り、情報を表示する準備が整いました。
3. カードが検出されると：
   - スピーカーを鳴らしサーバーに送信するQRコードを表示します。