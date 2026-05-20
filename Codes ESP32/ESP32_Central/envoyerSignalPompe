void envoyerSignalPompe() {
  String msg = "POMPE:" + String(demanderPompage ? "1" : "0");
  udpPompe.beginPacket(POMPE_IP, POMPE_PORT);
  udpPompe.print(msg);
  udpPompe.endPacket();
  Serial.println("Signal pompe envoye : " + msg);
  /* Remet a zero pour le prochain cycle la demande est remise a false apres envoi, elle est recalculee
  a chaque cycle dans appliquerRegles() */           
  demanderPompage = false;
}
