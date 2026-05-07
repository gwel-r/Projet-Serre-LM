// ================================================================
//  ESP32 Capteur Temperature - Serveur BLE
// ================================================================
#include <OneWire.h>
#include <DallasTemperature.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define SERVICE_UUID "55555555-5555-5555-5555-555555555555"
#define CHAR_UUID    "5555eeee-5555-5555-5555-5555eeeeeeee"
#define NOM_CAPTEUR  "ESP32_Temperature"
#define ONE_WIRE_BUS 4 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BLECharacteristic* pCarac;

// Ajouter cette classe AVANT setup()
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

  BLEServer* serveur = BLEDevice::createServer();
  serveur->setCallbacks(new CallbackServeur()); 
  BLEService* service  = serveur->createService(SERVICE_UUID);

  pCarac = service->createCharacteristic(
    CHAR_UUID,
    BLECharacteristic::PROPERTY_READ
  );

  pCarac->setValue("T:00.0");
  service->start();

  BLEAdvertising* pub = BLEDevice::getAdvertising();
  pub->addServiceUUID(SERVICE_UUID);
  pub->start();

  sensors.begin();
  Serial.println(String(NOM_CAPTEUR) + " demarre - en attente de connexions...");
}

void loop() {
  sensors.requestTemperatures();       // Demande une mesure
  float tempC = sensors.getTempCByIndex(0);   // Lit la température du 1er capteur
  //float temperature = random(18, 35); // temperature random pour les tests
  while (tempC == -127) {
    sensors.requestTemperatures();
    tempC = sensors.getTempCByIndex(0);
    Serial.println("Erreur capteur, nouvelle tentative...");
    delay(1000);
  }

  String msg = "T:" + String(tempC, 1);
  pCarac->setValue(msg.c_str());
  Serial.println("Temperature mise a jour : " + msg);
  delay(5000);
}