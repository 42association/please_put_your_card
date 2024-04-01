#include <M5StickCPlus2.h>
#include <MFRC522_I2C.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <qrcode.h>
#include <M5Unified.h>
#include <TFT_eSPI.h>
#include "image.h"  // 画像データを含むヘッダファイルをインクルード

const char* ssid = "";
const char* password = "";
const char* ntpServer = "";
const long  gmtOffset_sec = 9 * 3600;  // 日本標準時は、GMT+9
const int   daylightOffset_sec = 0;
const char* serverName = "";  // サーバーIP

MFRC522_I2C mfrc522(0x28, 0x3C, &Wire);    // Create MFRC522 instance. 创建MFRC522实例

bool showImage = false;  // 画像表示フラグ

void setup() {
  M5.begin();                 // Init M5Stack.
  M5.Power.begin();           // Init power
  M5.Lcd.setRotation(0);
  M5.Lcd.setTextSize(2);      // Set the text size to 2.

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.fillScreen(BLACK);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Wire.begin();               // Wire init, adding the I2C bus.
  mfrc522.PCD_Init();         // Init MFRC522.
}

void loop() {
  M5.update();  // ボタンの状態を更新

  M5.Lcd.drawBitmap(80, 200, epd_bitmap_42, 48, 34, WHITE);

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    M5.Lcd.println("Failed to obtain time");
    return;
  }
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.setTextSize(3);
  M5.Lcd.println(" Please\n\n put\n\n your\n\n card\n");

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {  // 如果没有读取到新的卡片
    delay(200);
    return;
  }

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  M5.Speaker.begin();
  M5.Speaker.tone(3000, 1000, 255);

  String getUrl = String(serverName) + uid;

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.qrcode(getUrl, 0, 0, 135, 6);

  M5.Lcd.setCursor(0, 150);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(getUrl);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      // M5.Lcd.println(response);
    } else {
      // M5.Lcd.printf("GET failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
  } else {
    M5.Lcd.println("WiFi Disconnected");
  }


  delay(5000);  // QRコードを表示する時間(ミリ秒)
  M5.Lcd.fillScreen(BLACK);
}