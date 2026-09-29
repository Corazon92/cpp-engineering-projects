#include "mesfonctions.h"
#include <iostream>
#include <limits>
#include "hungarian.h"

using namespace std;

/*
 * UrbanFlow – réseau de transport public
 * ──────────────────────────────────────
 * Trois profils d'utilisateurs :
 *   1. Usager        → trajet le plus rapide, le moins bondé, via une station
 *   2. Exploitant    → statistiques du réseau (occupancy, coupures, temps moyen)
 *   3. (extensible)  → Décideur / Analyste mobilité
 */



// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    // ── Chargement des données ────────────────────────────────────────────────
    cout << "Chargement des donnees...\n";

    vector<Station>    stations = charger_stations("stations_project2.csv");
    vector<Connection> connections = charger_connections("connections_project2.csv");
    vector<Coupure>    coupures = charger_coupures("coupures_lignes.csv");

    int N = (int)stations.size();
    cout << N << " stations chargees.\n\n";

    // ── Saisie de la date du trajet ───────────────────────────────────────────
    Date userdate;
    cout << "=== Date du trajet ===\n";
    userdate.jour = saisirEntier("  Jour   (1-31)  : ", 1, 31);
    userdate.mois = saisirEntier("  Mois   (1-12)  : ", 1, 12);
    userdate.annee = saisirEntier("  Annee  (ex: 2024) : ", 2000, 2100);

    // ── Construction de la matrice unifiée ───────────────────────────────────
    vector<vector<Arc>> mat = generer_matrice_reseau(stations, connections, coupures, userdate);

    // ── Menu principal ────────────────────────────────────────────────────────
    int menuPrincipal = 0;
    do
    {
        cout << "\n===============================\n";
        cout << "||                           ||\n";
        cout << "||        URBANFLOW          ||\n";
        cout << "||===========================||\n";
        cout << "||  1. Usager                ||\n";
        cout << "||  2. Exploitant du reseau  ||\n";
        cout << "||  0. Quitter               ||\n";
        cout << "===============================\n";
        menuPrincipal = saisirEntier("Votre choix : ", 0, 2);

        // ════════════════════════════════════════
        //  PROFIL USAGER
        // ════════════════════════════════════════
        if (menuPrincipal == 1)
        {
            // Affichage de la liste des stations
            cout << "\n--- Stations disponibles ---\n";
            for (int i = 0; i < N; i++)
                cout << "  [" << i + 1 << "] " << stations[i].nom << "\n";

            int depart = saisirEntier("\nStation de depart  (numero) : ", 1, N) - 1;
            int arrivee = saisirEntier("Station d'arrivee  (numero) : ", 1, N) - 1;

            if (depart == arrivee)
            {
                cout << "  -> Depart et arrivee identiques.\n";
                continue;
            }

            // Type de transport préféré
            TypeTransport typePrefere = saisirType();

            // Passage par une station intermédiaire ?
            char via;
            cout << "Passer par une station en particulier ? (o/n) : ";
            cin >> via;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (via == 'o' || via == 'O')
            {
                int inter = saisirEntier("Station intermediaire (numero) : ", 1, N) - 1;

                cout << "\n-- Trajet le plus rapide via " << stations[inter].nom << " --\n";
                Trajet tv = trajetViaStation(mat, stations, depart, inter, arrivee,
                    Critere::TEMPS, typePrefere);
                afficherResultat(tv, Critere::TEMPS, typePrefere);
            }
            else
            {
                // ── Trajet le plus rapide ──────────────────────────
                cout << "\n-- Trajet le plus rapide --\n";
                Trajet t1 = plusCourtTrajet(mat, stations, depart, arrivee,
                    Critere::TEMPS, typePrefere);
                afficherResultat(t1, Critere::TEMPS, typePrefere);

                // ── Trajet le moins bondé ──────────────────────────
                cout << "\n-- Trajet le moins bonde --\n";
                Trajet t2 = plusCourtTrajet(mat, stations, depart, arrivee,
                    Critere::OCCUPANCY, typePrefere);
                afficherResultat(t2, Critere::OCCUPANCY, typePrefere);

                // ── Trajet avec le moins d'arrêts ──────────────────
                cout << "\n-- Trajet avec le moins d'arrets --\n";
                Trajet t3 = plusCourtTrajet(mat, stations, depart, arrivee,
                    Critere::STATIONS, typePrefere);
                afficherResultat(t3, Critere::STATIONS, typePrefere);
            }
        }

        // ════════════════════════════════════════
        //  PROFIL EXPLOITANT
        // ════════════════════════════════════════
        else if (menuPrincipal == 2)
        {
            int menuExploitant = 0;
            do
            {
                cout << "\n-- Menu Exploitant --\n";
                cout << "  1. Station la plus frequentee\n";
                cout << "  2. Connexion la plus utilisee\n";
                cout << "  3. Nombre de coupures actives\n";
                cout << "  4. Temps moyen entre stations\n";
                cout << "  5. Afficher la matrice du reseau\n";
                cout << "  6. Analyser l'affluence\n";
                cout << "  7. Affecter les vehicules aux lignes  [Hongrois]\n";
                cout << "  0. Retour\n";
                menuExploitant = saisirEntier("Votre choix : ", 0, 7);

                switch (menuExploitant)
                {
                case 1: stationPlusFrequentee(mat, stations); break;
                case 2: connexionPlusUtilisee(mat, stations); break;
                case 3: nombreCoupures(mat);                  break;
                case 4: tempsMoyen(mat);                      break;
                case 5: afficherMat(mat);                     break;
                case 6: { vector<vector<float>> matNorm = normaliserOccupancy(mat); 
                    analyserMatriceNormalisee(matNorm,stations); 
                    break;
                }
                case 7: affecterVehicules(mat, stations);     break;
                default: break;
                }
            } while (menuExploitant != 0);
        }

    } while (menuPrincipal != 0);

    cout << "\nAu revoir !\n";
    return 0;
}