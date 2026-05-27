// ================================================================
//  ESP32 Capteur Humidite - Serveur BLE
//  ⚠️  Changer NOM_CAPTEUR avant de televerser sur chaque ESP32 :
//      "ESP32_Hum_Z1" → Zone 1
//      "ESP32_Hum_Z2" → Zone 2
//      "ESP32_Hum_Z3" → Zone 3
//      "ESP32_Hum_Z4" → Zone 4
// ================================================================

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

const int PIN_CAPTEUR = 34;//A changer en fonction d'où on branche le capteur

#define SERVICE_UUID "11111111-1111-1111-1111-111111111111"
#define CHAR_UUID    "1111aaaa-1111-1111-1111-1111aaaaaaaa"

// ⚠️ MODIFIER CE NOM POUR CHAQUE ESP32 CAPTEUR
#define NOM_CAPTEUR "ESP32_Hum_Z4"

BLECharacteristic* pCarac;

int AirValue = 2650; //air sec
int WaterValue = 80; //eau

class CallbackServeur : public BLEServerCallbacks {
  void onDisconnect(BLEServer* serveur) {
    // Redémarre l'annonce BLE après déconnexion
    BLEDevice::getAdvertising()->start();
    Serial.println("Client deconnecte - advertising redémarre");
  }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init(NOM_CAPTEUR);

  BLEServer*  serveur  = BLEDevice::createServer();
  serveur->setCallbacks(new CallbackServeur()); 
  BLEService* service  = serveur->createService(SERVICE_UUID);

  pCarac = service->createCharacteristic(
    CHAR_UUID,
    BLECharacteristic::PROPERTY_READ
  );

  pCarac->setValue("H:00.0");
  service->start();

  BLEAdvertising* pub = BLEDevice::getAdvertising();
  pub->addServiceUUID(SERVICE_UUID);
  pub->start();

  Serial.println(String(NOM_CAPTEUR) + " demarre - en attente de connexions...");
}

void loop() {
  int soilValue = analogRead(PIN_CAPTEUR);
  int moisturePercent =map(soilValue, AirValue, WaterValue, 0, 100);
  float humidite = constrain(moisturePercent, 0, 100);
  // float humidite = random(20, 80); essaie

  String msg = "H:" + String(humidite, 1);
  pCarac->setValue(msg.c_str());
  Serial.println("Humidite mise a jour : " + msg);

  delay(5000);
}
