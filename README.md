# please_put_your_card

## プロジェクトの概要
このプロジェクトは、M5StickCPlus2を活用してカードリーダーと表示システムを実現します。
ディスプレイを利用し、カードの読み取り、情報の表示、QRコードやシンプルな画像の表示などを簡単に行うことができます。
このシステムは、会員管理、イベント参加登録、出席管理など、様々な用途に応用可能です。

## ハードウェア要件
- [M5StickCPlus2](https://www.switch-science.com/products/9350)
- [RFIDカードリーダー](https://www.switch-science.com/products/8301)

### 接続方法
1. RFIDカードリーダーをM5StickCPlus2のGroveポートに接続します。
2. 以下の回路図を参考に、ハードウェアを接続してください。

![回路図](./circuit_diagram.png)

## ソフトウェア要件
- [Arduino IDE](https://www.arduino.cc/en/software)
- [VScode](https://code.visualstudio.com/)
- [platformio-ide](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide)

## インストール
1. **Arduino IDEセットアップ：** Arduino IDEをインストールし、M5StickCPlus2のボードマネージャーを追加します。詳しい手順は[こちら](https://docs.m5stack.com/en/quick_start/m5stickc_plus/arduino)を参照してください。

2. **VScodeをセットアップ:** VScodeをインストールします。

3. **PlatformIOセットアップ：** VScodeの拡張機能であるPlatformIO IDEをインストールし、プロジェクトの依存関係を解決します。詳しい手順は[こちら](https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation)を参照してください。

## 設定
- **WiFiセットアップ：** `secrets.h`ファイルにあなたのWiFiの認証情報を記入し、ネットワーク接続を有効にします。
- **サーバーセットアップ：** `secrets.h`サーバー設定
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