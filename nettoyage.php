<?php
$conn = new mysqli("localhost", "phpmyadmin", "pr20fss29", "serre");

if ($conn->connect_error) {
    echo "Erreur connexion : " . $conn->connect_error;
    exit;
}

$tables = ['temperature', 'humidite_1', 'humidite_2', 'humidite_3', 'humidite_4'];
$garder = 10;

foreach ($tables as $table) {
    $sql = "DELETE FROM `$table` WHERE id NOT IN (
                SELECT id FROM (
                    SELECT id FROM `$table` ORDER BY date_heure DESC LIMIT $garder
                ) AS tmp
            )";

    if ($conn->query($sql)) {
        $supprimees = $conn->affected_rows;
        echo "✅ $table : $supprimees ligne(s) supprimée(s)<br>";
    } else {
        echo "❌ Erreur sur $table : " . $conn->error . "<br>";
    }
}

$conn->close();
?>