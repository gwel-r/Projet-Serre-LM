void ouvrirTrappe (void)
{
  // ── Verin ────────────────────────────────────────────────────────
  const int motorPin3 = 18;
  const int motorPin4 = 19;
  const int speed = 255;
  // Forward
  analogWrite(motorPin3, speed);// avance
  analogWrite(motorPin4, 0);
  delay(10000);

  // Brief stop
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
  delay(200);
}