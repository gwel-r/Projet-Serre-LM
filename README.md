# Projet Serre LM — Serre Autonome 2026

Projet terminal STI2D SIN — Lycée La Mennais

---

## Présentation

**Projet Serre LM** est un système de serre entièrement autonome, développé dans le cadre de notre projet de terminal STI2D spécialité SIN. L'objectif est de gérer automatiquement l'irrigation et la ventilation, en s'appuyant sur un réseau de microcontrôleurs ESP32 communiquant par Bluetooth entre eux et par wifi avec un serveur. Toutes les données collectées sont accessibles en temps réel via un site web dédié.

---

## Auteurs

| Nom |
|---|
| Gweltaz |
| Hugo | 
| Nathan |

---

## Fonctionnalités

- Mesure en continu de la température intérieure de la serre
- Contrôle automatique de la ventilation (ouverture de trappes) selon les seuils de température
- Surveillance de l'humidité du sol sur 4 zones d'irrigation indépendantes (16 capteurs au total)
- Déclenchement automatique des électrovannes d'irrigation par zone selon l'humidité mesurée
- Surveillance du niveau d'eau de la réserve avec activation de la pompe conditionnée au niveau disponible
- Alimentation autonome de chaque module via panneaux solaires (18,2 V) et régulateurs de charge
- Centralisation des données sur un serveur Raspberry Pi 3B+ et sur un serveur
- Visualisation des données en temps réel via un site web (PHP / SQL)

---

## Architecture du système

Le système repose sur une architecture distribuée organisée en modules indépendants :

```
Panneaux Solaires (18,2 V)
    └── Module Chargeur Solaire → 3,3 V / 9 V / 12 V
            └── Alimentation des ESP32 et actionneurs

ESP32 Température
    └── Capteur température → Signal numérique → Bluetooth → Centrale (RPi)
            └── Commande : ouverture trappe 1 / trappe 2

ESP32 Humidité Zone 1 à 4 (×4)
    └── 4 capteurs analogiques d'humidité → Bluetooth → Centrale (RPi)
            └── Commande : ouverture électrovanne de zone (1 à 4)

ESP32 Pompe
    └── Sonde niveau d'eau → Bluetooth → Centrale (RPi)
            └── Commande : activation pompe + distribution générale

Centrale — Raspberry Pi 3B+
    └── Serveur de données (PHP / SQL)
            └── Site web accessible sur le réseau local
```

Le diagramme de bloc interne complet est disponible à la racine du dépôt (`diagramme_bloc_interne.pdf`).

---

## Technologies utilisées

| Couche | Technologies |
|---|---|
| Microcontrôleurs | ESP32, C++ (Arduino framework) |
| Centrale | Raspberry Pi 3B+ & serveur |
| Communication | Bluetooth (ESP32 ↔ RPi) & Wi-Fi |
| Serveur web | PHP, SQL |
| Interface utilisateur | HTML, CSS |

---

## Structure du dépôt

```
Projet-Serre-LM/
├──Codes ESP32/
│ └──esp32_temperature/            # Code ESP32 — module température / ventilation
│       └── esp32_temperature.ino
│    └── esp32_capteur_humidite/        
│       └── esp32_capteur_humidite_z1/ # Code ESP32 — module humidité zone 1
│           └── esp32_capteur_humidite_z1.ino
│       └── esp32_capteur_humidite_z2/ # Code ESP32 — module humidité zone 2
│           └── esp32_capteur_humidite_z2.ino
│       └── esp32_capteur_humidite_z3/ # Code ESP32 — module humidité zone 3
│           └── esp32_capteur_humidite_z3.ino
│       └── esp32_capteur_humidite_z4/ # Code ESP32 — module humidité zone 4
│           └── esp32_capteur_humidite_z4.ino
│    └── esp32_pompe/                  # Code ESP32 — module pompe / réserve d'eau
│       └── esp32_pompe.ino
├── diagramme_bloc_interne.pdf    # Schéma d'architecture du système
└── README.md
```

---

## Site web

Le site de visualisation des données est accessible à l'adresse suivante (réseau interne du lycée) :

[http://10.100.254.42/sites_eleves/Sites/LaMennais_Serre/](http://10.100.254.42/sites_eleves/Sites/LaMennais_Serre/)

---

## Contexte scolaire

Ce projet a été réalisé dans le cadre du **projet terminal de terminale STI2D, spécialité Systèmes d'Information et Numérique (SIN)**, au lycée La Mennais, année scolaire 2025-2026.

---

## Licence

Projet scolaire — tous droits réservés aux auteurs. Aucune réutilisation sans accord préalable.
