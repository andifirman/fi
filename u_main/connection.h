#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Wire.h>


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
//#define WIFI_SSID "codelabs"
//#define WIFI_PASSWORD "susahbro"

#define WIFI_SSID "test"
#define WIFI_PASSWORD "bismillahsukses"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAGmbgR_h-EMTPa6Umvi7H7QVhQSm6fmQM"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "clprecences@gmail.com"
#define USER_PASSWORD "code@labs011013"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://codelabs-precense-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Your Domain name with URL path or IP address with path
const char* registerNewUser = "https://codelabs-app.unikom.ac.id:5230/users/register-fingerprint";
const char* checkinAttendances = "https://codelabs-app.unikom.ac.id:5230/attendances/checkin-lab";
const char* checkoutAttendances = "https://codelabs-app.unikom.ac.id:5230/attendances/checkout-lab";

// Dummy
//const char* registerNewUser = "http://192.168.11.82:4444/users/register-fingerprint";
//const char* checkinAttendances = "http://192.168.11.82:4444/attendances/checkin-lab";
//const char* checkoutAttendances = "http://192.168.11.82:4444/attendances/checkout-lab";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;


// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
//FirebaseObject object;


bool signupOK = false;


// Variable to save USER UID
//String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
String checkOutPath;

// Database child nodes
String namePath = "/nama";
String snPath = "/nim";         // SN stand for Student Number
String spPath = "/prodi";       // SP stand for Study Program
String presencesPath = "/isPresences";       // SP stand for Study Program

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;


// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;



void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}





void startConnection() {
  Serial.begin(115200);

  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);


  // Update database path
  databasePath = "/codelabs/members";
}
