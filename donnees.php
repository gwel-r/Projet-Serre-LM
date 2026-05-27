<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$conn = new mysqli("localhost", "phpmyadmin", "pr20fss29", "serre");

// Verifie la connexion a la base de donnees
if ($conn->connect_error) {
    echo json_encode(["erreur" => "Connexion impossible"]);
    exit;
}

// Temperature
$resTemp = $conn->query("SELECT valeur FROM temperature ORDER BY date_heure DESC LIMIT 1");
$temp    = $resTemp ? $resTemp->fetch_assoc() : null;

// Humidite des 4 zones - une table par capteur
$humidites = [];
for ($i = 1; $i <= 4; $i++) {
    $table = "humidite_" . $i;
    $res   = $conn->query("SELECT valeur FROM $table ORDER BY date_heure DESC LIMIT 1");
    $row   = $res ? $res->fetch_assoc() : null;
    $humidites[$i] = $row ? round($row['valeur'], 1) : null;
}

$conn->close();

echo json_encode([
    "temperature" => $temp ? round($temp['valeur'], 1) : null,
    "humidites"   => $humidites
]);
?>