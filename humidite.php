<?php
$conn = new mysqli("localhost", "phpmyadmin", "pr20fss29", "serre");

// Verifie la connexion
if ($conn->connect_error) {
    echo "Erreur connexion";
    exit;
}

// Recupere le numero du capteur envoye par l'ESP32 (1, 2, 3 ou 4)
$capteur = intval($_GET['capteur']);
$valeur  = floatval($_GET['valeur']);

// Verifie que le numero est valide
if ($capteur < 1 || $capteur > 4) {
    echo "Numero de capteur invalide";
    exit;
}

// Choisit la bonne table selon le capteur
$table = "humidite_" . $capteur;

// Insere la valeur dans la bonne table
$sql = "INSERT INTO $table (valeur) VALUES ($valeur)";

if ($conn->query($sql)) {
    echo "OK";
} else {
    echo "Erreur insertion";
}

$conn->close();
?>
