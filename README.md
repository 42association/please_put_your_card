# please_put_your_card

<img src="https://github.com/kkaneta42/please_put_your_card/assets/135298886/b0a8457c-071e-4ee6-995f-e2fdd67354b6">

## 概要
M5StickCPlus2でNFCカードのUIDをサーバーに送信するためのQRコードを表示します。

<img src="https://github.com/42association/please_put_your_card/assets/135298886/81bf2243-7abb-4e85-882a-13a4d248f050" width="300px">

## ハードウェア要件
- [M5StickCPlus2](https://www.switch-science.com/products/9350)
- [RFIDカードリーダー](https://www.switch-science.com/products/8301)

## ソフトウェア要件
- [Arduino IDE](https://www.arduino.cc/en/software)
- [VScode](https://code.visualstudio.com/)
- [platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## インストール
1. **ドライバーをインストール：** [Driver MacOS](https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/drivers/CH9102_VCP_MacOS_v1.7.zip)をインストール

1. **Arduino IDEセットアップ：** [Arduino IDE](https://www.arduino.cc/en/software)をインストール

2. **VScodeをセットアップ：** [VScode](https://code.visualstudio.com/)をインストールします。

3. **PlatformIOセットアップ：** VScodeの拡張機能である[platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)をインストール

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
