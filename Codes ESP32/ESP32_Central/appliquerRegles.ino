// ── Application des regles ───────────────────────────────────────
void appliquerRegles() {
  // Gestion des 4 vannes d'humidite
  for (int i = 0; i < 4; i++) {
    if (humidites[i] >= 0) {
      if (humidites[i] < SEUIL_HUM) {
        digitalWrite(RELAIS_VANNE[i], HIGH);
        Serial.println("Vanne " + String(i + 1) + " ouverte (humidite : " + String(humidites[i]) + "%)");
        // NOUVEAU : si au moins une zone est seche, on demande le pompage
        demanderPompage = true;
      } else {
        digitalWrite(RELAIS_VANNE[i], LOW);
        Serial.println("Vanne " + String(i + 1) + " fermee (humidite : " + String(humidites[i]) + "%)");
      }
    }
  }
  // Gestion de la trappe temperature
  if (temperature >= 0) {
    if (temperature > SEUIL_TEMP) {
      digitalWrite(RELAIS_TRAPPE1, HIGH);
      digitalWrite(RELAIS_TRAPPE2, HIGH);
      Serial.println("Trappe ouverte (temperature : " + String(temperature) + "C)");
    } else {
      digitalWrite(RELAIS_TRAPPE1, LOW);
      digitalWrite(RELAIS_TRAPPE2, LOW);
      Serial.println("Trappe fermee (temperature : " + String(temperature) + "C)");
    }
  }
}
