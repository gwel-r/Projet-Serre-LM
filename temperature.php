<?php
$host = "localhost";
$db   = "serre";
$user = "phpmyadmin";
$pass = "pr20fss29";

$conn = new mysqli($host, $user, $pass, $db);

// Verification de la connexion
if ($conn->connect_error) {
    echo "Erreur connexion";
    exit;
}

$valeur = floatval($_GET['valeur']);

// Verification que la valeur est realiste
// -127 = erreur du capteur DS18B20
// On accepte uniquement des valeurs entre -20 et 60 degres
if ($valeur == -127 || $valeur < -20 || $valeur > 60) {
    echo "Valeur invalide : " . $valeur;
    exit;
}

// Insertion dans la base
$sql = "INSERT INTO temperature (valeur) VALUES ($valeur)";

if ($conn->query($sql)) {
    echo "OK";
} else {
    echo "Erreur insertion";
}

$conn->close();
?>
