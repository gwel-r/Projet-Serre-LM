void recevoirStatutPompe() {
  int packetSize = udpPompe.parsePacket();
  if (packetSize) {
    char buffer[64];
    int len = udpPompe.read(buffer, 63);
    if (len > 0) buffer[len] = '\0';

    String recu = String(buffer);
    Serial.println("Statut recu de l'ESP32 Pompe : " + recu);

    // NOUVEAU : mise a jour du statut de la vanne distante
    if (recu == "VANNE:1") {
      vanneDistanteOuverte = true;
      Serial.println("Electrovanne distante : OUVERTE");
    } else if (recu == "VANNE:0") {
      vanneDistanteOuverte = false;
      Serial.println("Electrovanne distante : FERMEE");
    }
  }
}
