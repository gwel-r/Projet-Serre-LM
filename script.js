async function miseAJour() {
    try {
        const response = await fetch('http://[IP_ADDRESS]/sites_eleves/Sites/LaMennais_Serre/donnees.php');
        const data = await response.json();

        if (data.erreur) {
            console.error(data.erreur);
            return;
        }

        /* TEMPÉRATURE */
        const temperature = data.temperature;
        const tempElement = document.getElementById("temp");
        if (tempElement) tempElement.textContent = (temperature !== null) ? temperature : "--";

        let trappeEtat = document.getElementById("trappeEtat");

        if (temperature > 25) {
            trappeEtat.textContent = "Ouverte";
            trappeEtat.style.background = "rgba(240,165,0,0.2)";
            trappeEtat.style.borderColor = "rgba(240,165,0,0.5)";
            trappeEtat.style.color = "#ffd166";
        } else {
            trappeEtat.textContent = "Fermée";
            trappeEtat.style.background = "rgba(77,175,125,0.15)";
            trappeEtat.style.borderColor = "rgba(77,175,125,0.3)";
            trappeEtat.style.color = "#6fcf97";
        }

        /* HUMIDITÉ + VANNES + COULEUR */
        // On boucle sur les zones existantes dans le HTML (ici id="h1", "h2"...)
        for (let i = 1; i <= 2; i++) {
            const hVal = data.humidites[i];
            const hSpan = document.getElementById("h" + i);
            const vSpan = document.getElementById("v" + i);
            const zone = document.getElementById("zone" + i);

            if (!hSpan || !vSpan || !zone) continue;

            let humTexte = "N/A";
            if (hVal !== null) {
                humTexte = hVal + "%";

                zone.classList.remove("zone-ouverte", "zone-fermee");

                // Logique simplifiée : < 40% = Sec (Vanne ouverte), sinon Fermée
                if (hVal < 40) {
                    vSpan.textContent = "Ouverte";
                    zone.classList.add("zone-ouverte");
                } else {
                    vSpan.textContent = "Fermée";
                    zone.classList.add("zone-fermee");
                }
            }
            hSpan.textContent = humTexte;
        }

    } catch (error) {
        console.error("Erreur lors de la récupération des données :", error);
    }
}

// Premier appel immédiat
miseAJour();

// Rafraîchissement régulier
setInterval(miseAJour, 30000); // Toutes les 30 secondes pour plus de réactivité
