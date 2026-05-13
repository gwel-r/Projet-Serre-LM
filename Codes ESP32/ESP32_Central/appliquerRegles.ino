void appliquerRegles(void) {
  // Vannes humidite
  for (int i = 0; i < 4; i++) {
    if (humidites[i] < 0) {
      Serial.println("Zone " + String(i+1) + " : pas de donnee, vanne inchangee");
      continue;
    }
    bool ouvrir = (humidites[i] < SEUIL_HUM);
    digitalWrite(RELAIS_VANNE[i], ouvrir ? HIGH : LOW);
    Serial.println("Zone " + String(i+1) + " : " + String(humidites[i]) + "% → vanne " + (ouvrir ? "OUVERTE" : "FERMEE"));
  }

  // Trappe temperature
  if (temperature < 0) {
    Serial.println("Temperature : pas de donnee, trappe inchangee");
    return;
  }
  if (temperature > SEUIL_TEMP){
    ouvrirTrappe();
  }
  else {
    fermerTrappe();
  }
  Serial.println("Temperature : " + String(temperature) + "°C → trappe " + (ouvrir ? "OUVERTE" : "FERMEE"));
}