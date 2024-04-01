#include <M5StickCPlus2.h>
#include <MFRC522_I2C.h>
#include <WiFi.h>
#include <time.h>
#include <qrcode.h>
#include <M5Unified.h>
#include <TFT_eSPI.h>
#include "image.h"
#include "pass.h"

bool displayText = true;
bool before = true;
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 9 * 3600;
const short daylightOffset_sec = 0;
const unsigned char QR_CODE_SIZE = 135;
const unsigned char QR_CODE_VERSION = 6;
const unsigned char SPEAKER_VOLUME = 225;
const short SPEAKER_FREQUENCY = 4261;
const unsigned char SPEAKER_DURATION = 100;
const short DISPLAY_DELAY = 5000;

MFRC522_I2C mfrc522(0x28, 0x3C, &Wire);
short Rotation = 0;

void setup() {
    M5.begin();
    M5.Imu.begin();
    M5.Power.begin();
    M5.Lcd.setRotation(0);
    M5.Lcd.setTextSize(2);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.fillScreen(BLACK);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Wire.begin();
    mfrc522.PCD_Init();
    M5.Speaker.begin();
    M5.Speaker.setVolume(SPEAKER_VOLUME);
}

void loop() {
	M5.update();

    if (M5.BtnA.wasReleased()) {
		before = displayText;
        displayText = !displayText;
		M5.Lcd.fillScreen(BLACK);
    }

    display_rotation_vertical();
    
    if (displayText) {
    	display_image();
        display_text(" Please\n\n put\n\n your\n\n card\n");
    } else {
    	display_image();
        display_text(" get\n\n your\n\n card\n\n uid\n\n");
    }
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
        delay(200);
        return;
    }
    String uid = get_uid();
    M5.Speaker.tone(SPEAKER_FREQUENCY, SPEAKER_DURATION, 1);
    String getUrl = serverName + uid;
	if (displayText) {
		display_qr_code_and_time(getUrl);
    } else {
    	display_uid_and_time(uid);  // uidを引数として渡す
    }
    delay(DISPLAY_DELAY);
    M5.Lcd.fillScreen(BLACK);
}

String get_uid() {
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    return uid;
}

void display_rotation_vertical() {
    float accX, accY, accZ;
    M5.Imu.getAccel(&accX, &accY, &accZ);
    short current = (accY > -0.3) ? 0 : 2;
    if (current != Rotation) {
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setRotation(current);
        Rotation = current;
    }
}

void display_image() {
    M5.Lcd.drawBitmap(80, 200, epd_bitmap_42, 48, 34, WHITE);
}

void display_text(const char* text) {
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.setTextSize(3);
    M5.Lcd.println(text);
}

void display_uid_and_time(String uid) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.println(uid);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void display_qr_code_and_time(String url) {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        M5.Lcd.println("Failed to obtain time");
        return;
    }
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.qrcode(url, 0, 0, QR_CODE_SIZE, QR_CODE_VERSION);
    M5.Lcd.setCursor(0, 150);
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}