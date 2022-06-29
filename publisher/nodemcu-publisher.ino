#include <Firebase.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <SPI.h>
#include <MFRC522.h>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <PubSubClient.h>

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D4  

// Set these to run example.
//#define FIREBASE_HOST "esad-speedviolation-default-rtdb.asia-southeast1.firebasedatabase.app"
//#define FIREBASE_AUTH "hXeIdFkEfnFCaoe2rGnXja8yKLl7q8czlDaquFfK"
#define WIFI_SSID "Redmi2"
#define WIFI_PASSWORD "kkppookkoop"

const char* mqtt_server = "192.168.0.121"; 
const char* mqtt_user = "admin1";
const char* mqtt_pass= "admin1";


// Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
//MFRC522 mfrc522Two mfrc522(SS_PIN1, RST_PIN1);

//PubSubclient instantiation
WiFiClient espClient;
PubSubClient client(espClient);


void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Traffic_Data", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


unsigned long t1 = 0;
unsigned long t2 = 0;
void setup() {
  Serial.begin(9600);		// Initialize serial communications with the PC
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("connected: ");
  Serial.println(WiFi.status());
  Serial.println(WiFi.localIP());

  //mqtt server setup for client
  client.setServer(mqtt_server, 1883);

  //Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();			// Init SPI bus
  mfrc522.PCD_Init();
  //mfrc522Two.PCD_Init();// Init MFRC522
  delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();
  //mfrc522Two.PCD_DumpVersionToSerial();// Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}
int n = 0;
void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;

  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  Serial.print(F("Card UID:"));
  String userid;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    userid += String(mfrc522.uid.uidByte[i], HEX);
    t1 = millis();
  }
  delay(1000);

  Serial.println(userid );
  Serial.println(t1);
  delay(3000);


// connecting to the mqtt client
  if (!client.connected()) {
    reconnect();
  }
  char msg[30];
  sprintf(msg,"%i %i",userid,t1);
  client.publish("traffic",msg);

//   String name = Firebase.pushInt(userid, t1);
//   // handle error
//   if (Firebase.failed()) {
//     Serial.print("pushing /logs failed:");
//     Serial.println(Firebase.error());
//     return;
//   }
//   Serial.print("pushed: /logs/");
//   Serial.println(name);
//   delay(1000);


  }