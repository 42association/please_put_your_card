#include <M5StickCPlus2.h>
#include <MFRC522_I2C.h>
#include <WiFi.h>
#include <time.h>
#include <HTTPClient.h>
#include <qrcode.h>
#include <M5Unified.h>
#include <TFT_eSPI.h>
#include "image.h" // 画像データを含むヘッダファイルをインクルード

const char* ssid = "";
const char* password = "";
const char* ntpServer = "";
const long  gmtOffset_sec = 9 * 3600; // 日本標準時は、GMT+9
const int   daylightOffset_sec = 0;
const char* serverName = ""; // サーバーIP
bool        showImage = false; // 画像表示フラグ
int         Rotation = 0;

MFRC522_I2C mfrc522(0x28, 0x3C, &Wire); // Create MFRC522 instance.

void setup() 
{
    M5.begin(); // Init M5Stack.
    M5.Imu.begin();
    M5.Power.begin(); // Init power
    M5.Lcd.setRotation(0);
    M5.Lcd.setTextSize(2); // Set the text size to 2.
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.fillScreen(BLACK);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Wire.begin(); // Wire init, adding the I2C bus.
    mfrc522.PCD_Init(); // Init MFRC522.
}

void loop()
{    
    display_rotation_vertical(); // 画面の向きを調整
    M5.Lcd.drawBitmap(80, 200, epd_bitmap_42, 48, 34, WHITE); //42
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextSize(3);
    M5.Lcd.println(" Please\n\n put\n\n your\n\n card\n");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    { // If no new cards are read
        delay(200);
        return;
    }
    String uid = get_uid();
    M5.Speaker.begin();
    M5.Speaker.tone(3000, 1000, 255);
    String getUrl = String(serverName) + uid;
    display_time(getUrl);
    delay(10000); // QRコードを表示する時間(ミリ秒)
    M5.Lcd.fillScreen(BLACK);
}

String get_uid()
{
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

void display_rotation_vertical()
{
    float accX, accY, accZ;
    M5.Imu.getAccel(&accX, &accY, &accZ);
    int current = (accY > -0.3) ? 0 : 2;
    if(current != Rotation)
        M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setRotation(current);
    Rotation = current;
}

void display_time(String getUrl)
{
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.qrcode(getUrl, 0, 0, 135, 6);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.setTextSize(2);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(getUrl);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0)
        {
            String response = http.getString();
            // M5.Lcd.println(response);
        }
        else
        {
            // M5.Lcd.printf("GET failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
    }
    else
    {
        M5.Lcd.println("WiFi Disconnected");
    }
}