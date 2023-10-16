const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 25200;
const int   daylightOffset_sec = 0;

int myHour;


void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  // Get hour data and store to integer for threshold
  myHour = timeinfo.tm_hour;
}
