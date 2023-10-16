// ------- Fingerprint Usage ----------

#include "keypad.h"
#include "enrollfingerprint.h"
#include "readfingerprint.h"
#include "connection.h"
#include "sendingData.h"
#include "serverTime.h"
#include "time.h"


// ------------------------------------
// ------------ Keypad ----------------
uint8_t tempKey;

bool currentState = false;




void setup()
{
  // Beginning wire and serial communication
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  // -----------------


  // Beginning connection
  startConnection();

  // Beginning fingerprint usage
  finger.begin(57600);

  // Checking if fingerprint is ready to use
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  // Starting network time protocol
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();



  // Beginning for keypad usage
  keyPad.loadKeyMap(keymap);

  Serial.println("Welcome to CodeLabs!");
  Serial.println("1. A for Admin Menu");
  Serial.println("2. B for Admin presences");
}

void loop() {
  // ---------------- Keypad ------------------
  ch = keyPad.getChar();



  if (ch) {
    if (ch == '*') {
      Serial.println("Aman");
    }
    else if (ch == 'A') {
      currentState = true;

      while (currentState == true) {
        Serial.println("Please insert the ID of new member: ");

        ch = keyPad.getChar();
        switch (ch) {
          case '0': case '1': case '2': case '3': case '4':
          case '5': case '6': case '7': case '8': case '9':
            pad += ch;
            Serial.print(ch);
            delay(300);
            break;
          case 'C':
            Serial.println("Input was erased");
            pad = "";
            break;
          case 'D':
            id = atoi(pad.c_str());
            delay(500);
            pad = "";

            Serial.println(id);

            addBio();
            getFingerprintEnroll();
            delay(500);

            if (enrollStatus == true) {
              addBio();
              sendingNewData(id);
            }

            currentState = false;
            break;
        }
      }

    } else {
      getFingerprintID();

      if (matchFinger == true) {
        //        check(fingerPresence);
//        checkOut(fingerPresence);
        //
        if (myHour >= 6 && myHour < 10) {
          checkIn(fingerPresence);
          Serial.println("Berhasil check-in! Semangat untuk hari ini ^^");
//          checkResponse();
          delay(3000);
        } else if (myHour >= 17 && myHour < 00) {
          checkOut(fingerPresence);
          Serial.println(myHour);
//          checkResponse();
          Serial.println("Berhasil check-out! Sampai jumpa besok ^^");
          delay(3000);
        } else {
          Serial.println("Gagal! Hahaha kasihan");
          Serial.println(myHour);
          checkOut(fingerPresence);
//          checkResponse();
          delay(3000);
        }
      }

      matchFinger = false;
    }
  }
}
