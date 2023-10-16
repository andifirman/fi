#include <HTTPClient.h>

// SN stand for Student Numer; SP stand for Student Program
String username, sn, sp = "";
String IDENTIFICATION = "FINGERPRINT";

bool done = false;
bool isPresences = false;

HTTPClient http;


void addBio() {

  while (Serial.available() == 0 && done == false)
  { //Wait for user input
    while (Serial.available() == 0) {}
    Serial.println("Masukkan nama: ");
    username = Serial.readString(); //Reading the Input string from Serial port.

    Serial.println("Masukkan NIM: ");
    while (Serial.available() == 0) {}
    sn = Serial.readString();

    done = true;
  }


}


void sendingNewData(uint8_t id) {
  // Kirim permintaan HTTP POST setiap 10 menit
  if ((millis() - lastTime) > timerDelay) {
    // Periksa status koneksi WiFi
    if (WiFi.status() == WL_CONNECTED) {
      //      WiFiClient client;
      HTTPClient http;

      // Alamat server dengan path atau alamat IP dengan path
      http.begin(registerNewUser);

      // Tentukan header content-type
      http.addHeader("Content-Type", "application/json");

      // Persiapkan data untuk dikirim dalam permintaan POST
      String httpRequestData = "{\"nim\": \"" + sn.substring(0, sn.length() - 1 ) + "\",\"fingerprint_id\":\"" + id + "\"}";
      Serial.println(httpRequestData);
      delay(2000);

      // Kirim permintaan HTTP POST
      int httpResponseCode = http.POST(httpRequestData);
      String response = http.getString(); // This line gets the response body as a String

      Serial.println("Response Body:");
      Serial.println(response);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Bebaskan sumber daya
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}



// Fungsi untuk validasi check-in
void checkIn(uint8_t id) {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    http.begin(checkinAttendances); //Specify the URL

    http.addHeader("Content-Type", "application/json");
    //    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String httpRequestData = "{\"identification\": \"" + IDENTIFICATION + "\",\"fingerprint_id\":\"" + id + "\"}";
    //    String httpRequestData = "identification=FINGERPRINT&fingerprint_id=" + id;

    delay(2000);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) { //Check for the returning code
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      Serial.println(httpRequestData);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
}


// Fungsi untuk validasi check-out
void checkOut(uint8_t id) {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    http.begin(checkoutAttendances); //Specify the URL


    http.addHeader("Content-Type", "application/json");
    String httpRequestData = "{\"identification\": \"" + IDENTIFICATION + "\",\"fingerprint_id\":\"" + id + "\"}";

    delay(2000);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) { //Check for the returning code
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end(); //Free the resources
  }
}

void checkResponse() {
  String response = http.getString();
  Serial.println(response);
  delay(2000);
}
