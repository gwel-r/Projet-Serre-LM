void envoyerHTTP(String url) {
  HTTPClient http;
  http.begin(url);
  int code = http.GET();
  http.end();
  Serial.println("HTTP " + String(code) + " → " + url);
}
