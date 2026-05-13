void fermerTrappe (void)
{
  // ── Verin ────────────────────────────────────────────────────────
  const int motorPin3 = 18;
  const int motorPin4 = 19;
  const int speed = 255;
  // Reverse
  analogWrite(motorPin3, 0);    //retourne 
  analogWrite(motorPin4, speed);
  delay(10000);

  // Brief stop
  analogWrite(motorPin3, 0);
  analogWrite(motorPin4, 0);
  delay(200);
}