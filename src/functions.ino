String get_uid()
{
	String uid = "";
	for (byte i = 0; i < mfrc522.uid.size; i++)
	{
		uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX);
	}
	uid.toUpperCase();
	return uid;
}

void display_rotation_vertical()
{
	float accX, accY, accZ;
	M5.Imu.getAccel(&accX, &accY, &accZ);
	short current = (accY > -0.3) ? 0 : 2;
	if (current != Rotation)
	{
		M5.Lcd.fillScreen(BLACK);
		M5.Lcd.setRotation(current);
		Rotation = current;
	}
}

void display_image()
{
	M5.Lcd.drawBitmap(80, 200, epd_bitmap_42, 48, 34, WHITE);
}

void display_text(const char *text)
{
	M5.Lcd.setCursor(0, 20);
	M5.Lcd.setTextSize(3);
	M5.Lcd.println(text);
}

void display_uid_and_time(String uid)
{
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo))
	{
		M5.Lcd.println("Failed to obtain time");
		return;
	}
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setTextSize(2);
	M5.Lcd.setCursor(0, 20);
	M5.Lcd.drawCentreString(uid, M5.Lcd.width() / 2, M5.Lcd.height() / 4, 1);
	M5.Lcd.setCursor(0, 150);
	M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void display_qr_code_and_time(String url)
{
	struct tm timeinfo;
	if (!getLocalTime(&timeinfo))
	{
		M5.Lcd.println("Failed to obtain time");
		return;
	}
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.qrcode(url, 0, 0, QR_CODE_SIZE, QR_CODE_VERSION);
	M5.Lcd.setCursor(0, 150);
	M5.Lcd.setTextSize(2);
	M5.Lcd.printf("%04d-%02d-%02d %02d:%02d:%02d\n", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

void post_activity(String uid) {
    HTTPClient http;
    http.begin(String(serverName) + "/activities");
    http.addHeader("Content-Type", "application/json");
	http.addHeader("Authorization", "Bearer " + String(apiKey));
    String postData = "{\"mac\": \"" + String(macAdr) + "\", \"uid\": \"" + uid + "\"}";
    int statusCode = http.POST(postData);
    if (statusCode == 200) {
        String response = http.getString();
        // M5.Lcd.println("Response: " + response);
        // M5.Lcd.println(message);
    } else {
        M5.Lcd.println("Error: " + String(statusCode));
    }
    http.end();
}
