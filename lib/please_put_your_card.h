#ifndef PLEASE_PUT_YOUR_CARD_H
#define PLEASE_PUT_YOUR_CARD_H

bool displayText = true;
bool before = true;
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 9 * 3600;
const short daylightOffset_sec = 0;
const unsigned char QR_CODE_SIZE = 135;
const unsigned char QR_CODE_VERSION = 6;
const unsigned char SPEAKER_VOLUME = 225;
const short SPEAKER_FREQUENCY = 4261;
const unsigned char SPEAKER_DURATION = 100;
const short DISPLAY_DELAY = 5000;
short Rotation = 0;

#include "lib/image.h"
#include "lib/.secrets.h"

#endif