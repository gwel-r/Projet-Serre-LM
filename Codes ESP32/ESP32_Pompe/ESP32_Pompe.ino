// ================================================================
//  ESP32_Pompe
//  - Recoit un signal de pompage (POMPE:0 ou POMPE:1) via UDP
//  - Lit la sonde de niveau d'eau (0 = niveau insuffisant, 1 = OK)
//  - Active la pompe ET ouvre l'electrovanne si :
//      signal pompage = 1  ET  niveau eau = 1
//  - Renvoie le statut de l'electrovanne au Central (VANNE:0 ou VANNE:1)
// ================================================================

#include <WiFi.h>
#include <WiFiUDP.h>

// ── WiFi ─────────────────────────────────────────────────────────
const char* SSID       = "TP_INDUS";
const char* PASSWORD   = "3rFui78x";
const char* CENTRAL_IP = "10.6.10.4";  // Remplacer par l'IP de l'ESP32 Central

// ── Ports UDP ────────────────────────────────────────────────────
const int LOCAL_PORT   = 1234;  // Ce port ecoute les messages du Central
const int CENTRAL_PORT = 1235;  // Port sur lequel le Central ecoute les reponses

// ── Broches ──────────────────────────────────────────────────────
const int PIN_SONDE = 34;  // Sonde niveau d'eau (HIGH = niveau OK, LOW = insuffisant)
const int PIN_POMPE = 26;  // Relais pompe       (LOW = active sur module relais standard)
const int PIN_VANNE = 27;  // Relais electrovanne (LOW = active sur module relais standard)

// ── Etat interne ─────────────────────────────────────────────────
bool signalPompage  = false;  // Signal recu du Central
bool vanneOuverte   = false;  // Etat actuel de l'electrovanne

WiFiUDP udp;
unsigned long dernierEnvoi = 0;
const int INTERVALLE_STATUT = 2000;  // Renvoie le statut toutes les 2 secondes

// ── Activation pompe + electrovanne ──────────────────────────────
void activerPompage() {
  digitalWrite(PIN_POMPE, LOW);   // Active le relais pompe
  digitalWrite(PIN_VANNE, LOW);   // Ouvre l'electrovanne
  vanneOuverte = true;
  Serial.println("Pompe ON - Electrovanne OUVERTE");
}

// ── Arret pompe + electrovanne ────────────────────────────────────
void desactiverPompage() {
  digitalWrite(PIN_POMPE, HIGH);  // Coupe le relais pompe
  digitalWrite(PIN_VANNE, HIGH);  // Ferme l'electrovanne
  vanneOuverte = false;
  Serial.println("Pompe OFF - Electrovanne FERMEE");
}

// ── Envoi du statut de la vanne au Central ───────────────────────
void envoyerStatut() {
  String msg = "VANNE:" + String(vanneOuverte ? "1" : "0");
  udp.beginPacket(CENTRAL_IP, CENTRAL_PORT);
  udp.print(msg);
  udp.endPacket();
  Serial.println("Statut envoye au Central : " + msg);
}

// ── Setup ────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Configuration des broches
  pinMode(PIN_POMPE, OUTPUT);
  pinMode(PIN_VANNE, OUTPUT);
  pinMode(PIN_SONDE, INPUT);

  // Securite : tout eteint au demarrage
  desactiverPompage();

  // Connexion WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connexion WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connecte - IP : " + WiFi.localIP().toString());

  // Demarrage ecoute UDP
  udp.begin(LOCAL_PORT);
  Serial.println("Ecoute UDP sur port " + String(LOCAL_PORT));
}

// ── Loop ─────────────────────────────────────────────────────────
void loop() {

  // 1. Reception d'un message UDP du Central
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char buffer[64];
    int len = udp.read(buffer, 63);
    if (len > 0) buffer[len] = '\0';

    String recu = String(buffer);
    Serial.println("Message recu : " + recu);

    if (recu == "POMPE:1") {
      signalPompage = true;
    } else if (recu == "POMPE:0") {
      signalPompage = false;
    }
  }

  // 2. Lecture de la sonde de niveau d'eau
  //    HIGH = niveau suffisant, LOW = reservoir vide ou insuffisant
  bool niveauOK = (digitalRead(PIN_SONDE) == HIGH);

  if (!niveauOK) {
    Serial.println("Niveau d'eau insuffisant - pompage bloque");
  }

  // 3. Logique de controle
  //    Les deux conditions doivent etre vraies pour activer le pompage
  if (signalPompage && niveauOK) {
    if (!vanneOuverte) activerPompage();
  } else {
    if (vanneOuverte) desactiverPompage();
  }

  // 4. Envoi periodique du statut au Central
  if (millis() - dernierEnvoi > INTERVALLE_STATUT) {
    dernierEnvoi = millis();
    envoyerStatut();
  }

  delay(100);
}
