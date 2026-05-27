<?php
$conn = new mysqli("localhost", "phpmyadmin", "pr20fss29", "serre");

$temp = $conn->query("SELECT valeur, date_heure FROM temperature ORDER BY date_heure DESC LIMIT 1")->fetch_assoc();
$humidites = [];
for ($i = 1; $i <= 4; $i++) {
  $table = "humidite_" . $i;
  $res = $conn->query("SELECT valeur, date_heure FROM $table ORDER BY date_heure DESC LIMIT 1");
  $humidites[$i] = $res ? $res->fetch_assoc() : null;
}
?>
<!DOCTYPE html>
<html lang="fr">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="30"> <!-- Rafraichit toutes les 30s -->
  <title> Ma Serre</title>
  <style>
    body { font-family: Arial, sans-serif; background: #e8f5e9; text-align: center; padding: 30px; }
    h1 { color: #2e7d32; }
    .carte { background: white; border-radius: 15px; padding: 20px; margin: 15px auto;
             max-width: 300px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
    .valeur { font-size: 2.5em; font-weight: bold; color: #388e3c; }
    .label { font-size: 1em; color: #777; margin-top: 5px; }
    .date { font-size: 0.8em; color: #aaa; }
  </style>
</head>
<body>
  <h1> Tableau de bord - Ma Serre</h1>

  <div class="carte">
    <div class="valeur"> <?= $temp ? round($temp['valeur'], 1) . "°C" : "N/A" ?></div>
    <div class="label">Temperature</div>
    <div class="date"><?= $temp ? $temp['date_heure'] : "" ?></div>
  </div>

  <?php for ($i = 1; $i <= 4; $i++): ?>
  <div class="carte">
    <div class="valeur"> <?= $humidites[$i] ? round($humidites[$i]['valeur'], 1) . "%" : "N/A" ?></div>
    <div class="label">Humidite sol #<?= $i ?></div>
    <div class="date"><?= $humidites[$i] ? $humidites[$i]['date_heure'] : "" ?></div>
  </div>
  <?php endfor; ?>

</body>
</html>
