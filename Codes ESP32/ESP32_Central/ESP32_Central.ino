// ================================================================
//      Penser a selectionner :
//      Outils → Partition Scheme → Huge APP (3MB No OTA) 
// ================================================================

#include <BLEDevice.h>
#include <BLEClient.h>
#include <BLEScan.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DallasTemperature.h>

// ── WiFi ─────────────────────────────────────────────────────────
const char* SSID         = "TP_INDUS";
const char* PASSWORD     = "3rFui78x";
const char* RASPBERRY_IP = "10.6.10.56";
const char* SERVEUR_IP = "10.100.254.42"; 

// ── Seuils de declenchement ──────────────────────────────────────
const float SEUIL_HUM  = 30.0;  // Ouvre vanne si humidite < 30%    a changer si la valeur est top basse ou trop haute
const float SEUIL_TEMP = 25.0;  // Ouvre trappe si temperature > 25 degres

// ── Broches relais ───────────────────────────────────────────────
const int RELAIS_VANNE[4] = {26, 27, 14, 12};
const int RELAIS_TRAPPE1   = 13;
const int RELAIS_TRAPPE2   = 11;

// ── UUIDs BLE ────────────────────────────────────────────────────
#define SERVICE_HUM  "11111111-1111-1111-1111-111111111111"
#define CHAR_HUM     "1111aaaa-1111-1111-1111-1111aaaaaaaa"
#define SERVICE_TEMP "55555555-5555-5555-5555-555555555555"
#define CHAR_TEMP    "5555eeee-5555-5555-5555-5555eeeeeeee"

// ── Noms Bluetooth des capteurs ──────────────────────────────────
const char* NOMS_HUM[4] = {
  "ESP32_Hum_Z1",
  "ESP32_Hum_Z2",
  "ESP32_Hum_Z3",
  "ESP32_Hum_Z4"
};

const int motorPin3 = 9;
const int motorPin4 = 10;
int ouvrir = LOW; 

// ── Donnees recues ───────────────────────────────────────────────
float humidites[4] = {-1, -1, -1, -1};
float temperature  = -1;

// ── Lecture BLE ──────────────────────────────────────────────────
String lireBluetooth(const char* nom, const char* serviceUUID, const char* charUUID) {
  Serial.println("Recherche : " + String(nom));

  BLEScan* scan = BLEDevice::getScan();
  scan->setActiveScan(true);
  BLEScanResults* results = scan->start(7, false); // valeur 10 = temps de recherche pour trouver une valeur (temperature et humidite)

  BLEAddress adresse("");
  bool trouve = false;

  for (int i = 0; i < results->getCount(); i++) {
    BLEAdvertisedDevice appareil = results->getDevice(i);
    if (String(appareil.getName().c_str()) == String(nom)) {
      adresse = appareil.getAddress();
      trouve  = true;
      break;
    }
  }
  scan->clearResults();

  if (!trouve) {
    Serial.println("Non trouve : " + String(nom));
    return "";
  }

  BLEClient* client = BLEDevice::createClient();
  if (!client->connect(adresse)) {
    Serial.println("Connexion echouee : " + String(nom));
    return "";
  }

  BLERemoteService* service = client->getService(serviceUUID);
  if (!service) {
    client->disconnect();
    return "";
  }

  BLERemoteCharacteristic* carac = service->getCharacteristic(charUUID);
  if (!carac) {
    client->disconnect();
    return "";
  }

  String valeur = carac->readValue().c_str();
  client->disconnect();
  Serial.println("Recu de " + String(nom) + " : " + valeur);
  return valeur;
}

// ── Setup ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  // Verin
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Relais - tout ferme au demarrage
  for (int i = 0; i < 4; i++) {
    pinMode(RELAIS_VANNE[i], OUTPUT);
    digitalWrite(RELAIS_VANNE[i], LOW);
  }
  pinMode(RELAIS_TRAPPE1, OUTPUT);
  digitalWrite(RELAIS_TRAPPE1, LOW);
  pinMode(RELAIS_TRAPPE2, OUTPUT);
  digitalWrite(RELAIS_TRAPPE2, LOW);


  // Bluetooth
  BLEDevice::init("ESP32_Central");

  // WiFi
  Serial.print("Connexion WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecte - IP : " + WiFi.localIP().toString());
}

// ── Loop ─────────────────────────────────────────────────────────
void loop() {
  Serial.println("=== Nouveau cycle ===");

  // 1. Lecture des 4 capteurs humidite
  for (int i = 0; i < 4; i++) {
    String msg = lireBluetooth(NOMS_HUM[i], SERVICE_HUM, CHAR_HUM);
    if (msg.startsWith("H:")) humidites[i] = msg.substring(2).toFloat();
    delay(1000);
  }

  // 2. Lecture capteur temperature
  String msgTemp = lireBluetooth("ESP32_Temperature", SERVICE_TEMP, CHAR_TEMP);
  if (msgTemp.startsWith("T:")) temperature = msgTemp.substring(2).toFloat();

  // 3. Application des regles → vannes et trappe
  appliquerRegles();

  // 4. Envoi des donnees au Raspberry Pi
  //envoyerRaspberry();     //plus utile car on a le serveur
  envoyerserveur();

  Serial.println("=== Fin cycle - pause 6s ===");
  delay(6000);
}
