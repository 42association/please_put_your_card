#include <M5StickCPlus2.h>
#include <MFRC522_I2C.h>
#include <WiFi.h>
#include <time.h>
#include <qrcode.h>
#include <M5Unified.h>
#include <TFT_eSPI.h>

#include "lib/please_put_your_card.h"

MFRC522_I2C mfrc522(0x28, 0x3C, &Wire);

void setup()
{
	M5.begin();
	M5.Imu.begin();
	M5.Power.begin();
	M5.Lcd.setRotation(0);
	M5.Lcd.setTextSize(2);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
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

void loop()
{
	M5.update();

	if (M5.BtnA.wasReleased())
	{
		before = displayText;
		displayText = !displayText;
		M5.Lcd.fillScreen(BLACK);
	}
	display_rotation_vertical();
	display_image();
	if (displayText)
		display_text(" Please\n\n put\n\n your\n\n card\n");
	else
		display_text(" get\n\n your\n\n card\n\n uid\n\n");
	if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
	{
		delay(200);
		return;
	}
	String uid = get_uid();
	M5.Speaker.tone(SPEAKER_FREQUENCY, SPEAKER_DURATION, 1);
	String getUrl = serverName + uid;
	if (displayText)
		display_qr_code_and_time(getUrl);
	else
		display_uid_and_time(uid);
	delay(DISPLAY_DELAY);
	M5.Lcd.fillScreen(BLACK);
}