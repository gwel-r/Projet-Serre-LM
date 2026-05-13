void envoyerserveur(void) {
    if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi deconnecte - envoi impossible");
    return;
  }

  if (temperature >= 0)
    envoyerHTTP("http://" + String(SERVEUR_IP) + "/temperature.php?valeur=" + String(temperature, 1));

  for (int i = 0; i < 4; i++) {
    if (humidites[i] < 0) continue;
    envoyerHTTP("http://" + String(SERVEUR_IP)
      + "/humidite.php?capteur=" + String(i+1)
      + "&valeur=" + String(humidites[i], 1));
  }
}