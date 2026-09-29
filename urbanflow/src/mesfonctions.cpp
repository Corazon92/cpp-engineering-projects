#include "mesfonctions.h"
#include "hungarian.h"
// ─────────────────────────────────────────────
//  Utilitaires
// ─────────────────────────────────────────────

Date stringToDate(const std::string& str)
{
    Date d;
    sscanf_s(str.c_str(), "%d-%d-%d", &d.annee, &d.mois, &d.jour);
    return d;
}

TypeTransport stringToType(const string& s)
{
    if (s == "metro" || s == "Metro" || s == "METRO") return TypeTransport::METRO;
    if (s == "tram" || s == "Tram" || s == "TRAM")  return TypeTransport::TRAM;
    return TypeTransport::BUS; // valeur par défaut
}

string typeToString(TypeTransport t)
{
    switch (t)
    {
    case TypeTransport::METRO: return "metro";
    case TypeTransport::TRAM:  return "tram";
    case TypeTransport::BUS:   return "bus";
    default:                   return "tous";
    }
}
// ── Petite aide pour saisir un entier proprement ──────────────────────────────
int saisirEntier(const string& message, int min, int max)
{
    int val;
    while (true)
    {
        cout << message;
        cin >> val;
        if (cin.fail() || val < min || val > max)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  -> Valeur invalide. Entrez un entier entre "
                << min << " et " << max << ".\n";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
    }
}

// ── Saisie du type de transport préféré ──────────────────────────────────────
TypeTransport saisirType()
{
    cout << "\nType de transport prefere ?\n"
        << "  1. Metro\n"
        << "  2. Bus\n"
        << "  3. Tram\n"
        << "  4. Aucune preference\n";
    int choix = saisirEntier("Votre choix : ", 1, 4);
    switch (choix)
    {
    case 1: return TypeTransport::METRO;
    case 2: return TypeTransport::BUS;
    case 3: return TypeTransport::TRAM;
    default: return TypeTransport::TOUS;
    }
}


// ─────────────────────────────────────────────
//  Chargement des fichiers CSV
// ─────────────────────────────────────────────

vector<Station> charger_stations(const string& nomFichier)
{
    ifstream fichier(nomFichier);
    vector<Station> all_stations;
    string ligne;

    getline(fichier, ligne); // en-tête

    while (getline(fichier, ligne))
    {
        Station station;
        stringstream ss(ligne);
        getline(ss, station.id, ',');
        getline(ss, station.nom, ',');
        all_stations.push_back(station);
    }

    fichier.close();
    return all_stations;
}

vector<Connection> charger_connections(const string& nomFichier)
{
    ifstream fichier(nomFichier);
    vector<Connection> all_connections;
    string ligne;

    getline(fichier, ligne); // en-tête

    while (getline(fichier, ligne))
    {
        Connection connection;
        stringstream ss(ligne);
        getline(ss, connection.from, ',');
        getline(ss, connection.to, ',');
        getline(ss, connection.temps, ',');
        getline(ss, connection.type, ',');
        getline(ss, connection.frequence, ',');
        getline(ss, connection.occupancy, ',');
        all_connections.push_back(connection);
    }

    fichier.close();
    return all_connections;
}

vector<Coupure> charger_coupures(const string& nomFichier)
{
    ifstream fichier(nomFichier);
    vector<Coupure> all_coupures;
    string ligne;

    getline(fichier, ligne); // en-tête

    while (getline(fichier, ligne))
    {
        Coupure coupure;
        string date_debut_str, date_fin_str;
        stringstream ss(ligne);

        getline(ss, coupure.from, ',');
        getline(ss, coupure.to, ',');
        getline(ss, date_debut_str, ',');
        getline(ss, date_fin_str, ',');

        coupure.date_debut = stringToDate(date_debut_str);
        coupure.date_fin = stringToDate(date_fin_str);

        all_coupures.push_back(coupure);
    }

    fichier.close();
    return all_coupures;
}

// ─────────────────────────────────────────────
//  Construction de la matrice unifiée
// ─────────────────────────────────────────────

/*
 * generer_matrice_reseau
 * ──────────────────────
 * Construit en une seule passe la matrice N×N d'Arc.
 * Chaque Arc regroupe :
 *   - existe    : il y a bien une liaison entre les deux stations
 *   - temps     : durée du trajet en minutes
 *   - occupancy : nombre moyen de passagers sur cette liaison
 *   - coupe     : la liaison est interrompue à la date donnée
 *
 * Avantage : on remplace les 4 matrices int séparées par une seule
 * structure, ce qui simplifie tous les algorithmes en aval.
 */
vector<vector<Arc>> generer_matrice_reseau(
    const vector<Station>& stations,
    const vector<Connection>& connections,
    const vector<Coupure>& coupures,
    Date                      date_trajet)
{
    int N = (int)stations.size();

    // Arc par défaut : pas de liaison, tous les champs à zéro/false
    Arc arcVide = { false, 0, 0, false };
    vector<vector<Arc>> mat(N, vector<Arc>(N, arcVide));

    // Remplissage à partir des connexions
    for (const auto& conn : connections)
    {
        int from = stoi(conn.from) - 1;
        int to = stoi(conn.to) - 1;

        mat[from][to].existe = true;
        mat[from][to].temps = stoi(conn.temps);
        mat[from][to].occupancy = stoi(conn.occupancy);
        mat[from][to].type = stringToType(conn.type);
    }

    // Application des coupures actives à la date donnée
    for (const auto& c : coupures)
    {
        int from = stoi(c.from) - 1;
        int to = stoi(c.to) - 1;

        if (date_trajet >= c.date_debut && date_trajet <= c.date_fin)
            mat[from][to].coupe = true;
    }

    return mat;
}

// ─────────────────────────────────────────────
//  Affichage de la matrice (debug)
// ─────────────────────────────────────────────

void afficherMat(const vector<vector<Arc>>& mat)
{
    int N = (int)mat.size();
    cout << "     ";
    for (int j = 0; j < N; j++)
        cout << j + 1 << "\t";
    cout << endl;

    for (int i = 0; i < N; i++)
    {
        cout << i + 1 << " -> ";
        for (int j = 0; j < N; j++)
        {
            if (!mat[i][j].existe)
                cout << "-\t";
            else if (mat[i][j].coupe)
                cout << "X\t";
            else
                cout << mat[i][j].temps << "mn/"
                << typeToString(mat[i][j].type) << "\t";
        }
        cout << endl;
    }
}

// ─────────────────────────────────────────────
//  Algorithme de Dijkstra générique
// ─────────────────────────────────────────────

/*
 * plusCourtTrajet
 * ───────────────
 * Calcule le meilleur chemin selon le critère choisi.
 *
 * Si typePrefere != TOUS :
 *   1re passe  → Dijkstra en n'empruntant QUE les arcs du bon type.
 *                Si un chemin existe → retourné avec hors_type = 0.
 *   2e passe   → Dijkstra sur tous les arcs mais en pénalisant
 *                fortement les arcs hors type (poids × PENALITE).
 *                On favorise ainsi au maximum le type voulu, tout en
 *                garantissant qu'un chemin sera trouvé s'il existe.
 *                On compte ensuite les segments hors type dans le
 *                chemin final → stocké dans trajet.hors_type.
 *
 * Si typePrefere == TOUS → comportement identique à l'ancienne version.
 */
static const int PENALITE = 100; // malus appliqué aux arcs hors type

// Noyau Dijkstra interne : filtre optionnel sur le type d'arc
static Trajet dijkstra(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int arrivee,
    Critere critere,
    TypeTransport filtre)          // TOUS = pas de filtre
{
    Trajet trajet;
    int N = (int)mat.size();
    const int INF = 1000000;

    vector<int> dist(N, INF);
    vector<int> visite(N, 0);
    vector<int> precedent(N, -1);
    dist[depart] = 0;

    for (int k = 0; k < N; k++)
    {
        int u = -1, minDist = INF;
        for (int i = 0; i < N; i++)
            if (!visite[i] && dist[i] < minDist) { minDist = dist[i]; u = i; }
        if (u == -1) break;
        visite[u] = 1;

        for (int v = 0; v < N; v++)
        {
            const Arc& a = mat[u][v];
            if (!a.existe || a.coupe) continue;

            // Filtre strict : on ignore les arcs hors type demandé
            if (filtre != TypeTransport::TOUS && a.type != filtre) continue;

            int poids = 0;
            switch (critere)
            {
            case Critere::TEMPS:     poids = a.temps;     break;
            case Critere::OCCUPANCY: poids = a.occupancy; break;
            case Critere::STATIONS:  poids = 1;           break;
            }

            int alt = dist[u] + poids;
            if (alt < dist[v]) { dist[v] = alt; precedent[v] = u; }
        }
    }

    if (dist[arrivee] == INF)
    {
        trajet.chemin = "Aucun trajet disponible";
        return trajet;
    }

    vector<int> chemin;
    for (int v = arrivee; v != -1; v = precedent[v]) chemin.push_back(v);

    for (int i = (int)chemin.size() - 1; i >= 0; i--)
    {
        trajet.chemin += stations[chemin[i]].nom;
        if (i > 0) trajet.chemin += " -> ";
    }
    trajet.data = dist[arrivee];
    return trajet;
}

// Dijkstra avec pénalité (passe souple) + comptage hors_type
static Trajet dijkstraPenalise(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int arrivee,
    Critere critere,
    TypeTransport typePrefere)
{
    Trajet trajet;
    int N = (int)mat.size();
    const int INF = 1000000000;

    vector<int> dist(N, INF);
    vector<int> visite(N, 0);
    vector<int> precedent(N, -1);
    dist[depart] = 0;

    for (int k = 0; k < N; k++)
    {
        int u = -1, minDist = INF;
        for (int i = 0; i < N; i++)
            if (!visite[i] && dist[i] < minDist) { minDist = dist[i]; u = i; }
        if (u == -1) break;
        visite[u] = 1;

        for (int v = 0; v < N; v++)
        {
            const Arc& a = mat[u][v];
            if (!a.existe || a.coupe) continue;

            int poids = 0;
            switch (critere)
            {
            case Critere::TEMPS:     poids = a.temps;     break;
            case Critere::OCCUPANCY: poids = a.occupancy; break;
            case Critere::STATIONS:  poids = 1;           break;
            }

            // Pénalité si l'arc n'est pas du type préféré
            if (a.type != typePrefere)
                poids = poids * PENALITE;

            int alt = dist[u] + poids;
            if (alt < dist[v]) { dist[v] = alt; precedent[v] = u; }
        }
    }

    if (dist[arrivee] == INF)
    {
        trajet.chemin = "Aucun trajet disponible";
        return trajet;
    }

    // Reconstruction + comptage des segments hors type
    vector<int> chemin;
    for (int v = arrivee; v != -1; v = precedent[v]) chemin.push_back(v);

    int horsType = 0;
    for (int i = (int)chemin.size() - 1; i >= 0; i--)
    {
        trajet.chemin += stations[chemin[i]].nom;
        if (i > 0)
        {
            trajet.chemin += " -> ";
            int u = chemin[i], v = chemin[i - 1];
            if (mat[u][v].type != typePrefere) horsType++;
        }
    }

    // On recalcule data SANS pénalité pour afficher la vraie valeur
    Trajet sans_penalite = dijkstra(mat, stations, depart, arrivee, critere,
        TypeTransport::TOUS);
    trajet.data = sans_penalite.data;
    trajet.hors_type = horsType;
    return trajet;
}

Trajet plusCourtTrajet(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int arrivee,
    Critere critere,
    TypeTransport typePrefere)
{
    // Pas de préférence → Dijkstra classique
    if (typePrefere == TypeTransport::TOUS)
        return dijkstra(mat, stations, depart, arrivee, critere, TypeTransport::TOUS);

    // 1re passe : trajet 100% dans le type demandé
    Trajet strict = dijkstra(mat, stations, depart, arrivee, critere, typePrefere);
    if (strict.chemin != "Aucun trajet disponible")
        return strict; // hors_type reste 0

    // 2e passe : on pénalise les arcs hors type mais on autorise tout
    return dijkstraPenalise(mat, stations, depart, arrivee, critere, typePrefere);
}

/*
 * trajetViaStation
 * ────────────────
 * Trajet contraint : départ → inter → arrivée
 * On enchaîne deux Dijkstra et on concatène les chemins.
 */
Trajet trajetViaStation(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int inter, int arrivee,
    Critere critere,
    TypeTransport typePrefere)
{
    Trajet t1 = plusCourtTrajet(mat, stations, depart, inter, critere, typePrefere);
    Trajet t2 = plusCourtTrajet(mat, stations, inter, arrivee, critere, typePrefere);

    Trajet resultat;

    if (t1.chemin == "Aucun trajet disponible" ||
        t2.chemin == "Aucun trajet disponible")
    {
        resultat.chemin = "Aucun trajet disponible via cette station";
        return resultat;
    }

    // Concaténation sans dupliquer le nom de la station intermédiaire
    string prefixeInter = stations[inter].nom + " -> ";
    string suite = t2.chemin.substr(prefixeInter.size());
    resultat.chemin = t1.chemin + " -> " + suite;
    resultat.data = t1.data + t2.data;
    resultat.hors_type = t1.hors_type + t2.hors_type;
    return resultat;
}

// ─────────────────────────────────────────────
//  Fonctions d'analyse pour l'exploitant
// ─────────────────────────────────────────────

void stationPlusFrequentee(const vector<vector<Arc>>& mat, const vector<Station>& stations)
{
    int N = (int)mat.size();
    int maxFreq = 0, stationMax = -1;

    for (int i = 0; i < N; i++)
    {
        int somme = 0;
        for (int j = 0; j < N; j++)
            somme += mat[i][j].occupancy;

        if (somme > maxFreq)
        {
            maxFreq = somme;
            stationMax = i;
        }
    }

    if (stationMax == -1) { cout << "Aucune donnee." << endl; return; }

    cout << "Station la plus frequentee : " << stations[stationMax].nom << endl;
    cout << "Total passagers departs    : " << maxFreq << endl;
}

void connexionPlusUtilisee(const vector<vector<Arc>>& mat, const vector<Station>& stations)
{
    int N = (int)mat.size();
    int maxOcc = 0, from = -1, to = -1;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j].occupancy > maxOcc)
            {
                maxOcc = mat[i][j].occupancy;
                from = i;
                to = j;
            }

    if (from == -1) { cout << "Aucune donnée." << endl; return; }

    cout << "Connexion la plus utilisee : "
        << stations[from].nom << " -> " << stations[to].nom << endl;
    cout << "Passagers                  : " << maxOcc << endl;
}

void nombreCoupures(const vector<vector<Arc>>& mat)
{
    int N = (int)mat.size();
    int count = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j].coupe) count++;

    cout << "Connexions coupees : " << count << endl;
}

void tempsMoyen(const vector<vector<Arc>>& mat)
{
    int N = (int)mat.size();
    int somme = 0, count = 0;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j].existe && !mat[i][j].coupe)
            {
                somme += mat[i][j].temps;
                count++;
            }

    if (count > 0)
        cout << "Temps moyen entre stations : " << somme / count << " minutes" << endl;
    else
        cout << "Aucune connexion active." << endl;
}

vector<vector<float>> normaliserOccupancy(const vector<vector<Arc>>& mat)
{
    int N = mat.size();

    int minVal = 1000000;
    int maxVal = -1;

    // 1. Trouver min et max
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (mat[i][j].existe && !mat[i][j].coupe)
            {
                int val = mat[i][j].occupancy;

                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
            }
        }
    }

    cout << "Min occupancy : " << minVal << endl;
    cout << "Max occupancy : " << maxVal << endl;

    // 2. Matrice normalisée
    vector<vector<float>> matNorm(N, vector<float>(N, 0.0));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (mat[i][j].existe && !mat[i][j].coupe)
            {
                int val = mat[i][j].occupancy;

                matNorm[i][j] = float(val - minVal) / (maxVal - minVal);
            }
        }
    }

    return matNorm;
}
void analyserMatriceNormalisee(const vector<vector<float>>& matNorm, const vector<Station>& stations)
{
    int N = matNorm.size();
    int countTotal = 0;
    int countEleve = 0;

    cout << "\n--- Connexions tres fréquentees (seuil > 80%) ---\n";

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (matNorm[i][j] > 0)
            {
                countTotal++;
                if (matNorm[i][j] > 0.8)
                {
                    countEleve++;
                    cout << "  " << stations[i].nom << " --> " << stations[j].nom
                        << "  (" << int(matNorm[i][j] * 100) << "%)\n";
                }
            }
        }
    }

    float pourcentage = countTotal > 0 ? (float)countEleve / countTotal * 100 : 0;
    cout << "\nTotal : " << countEleve << " connexion(s) saturee(s) sur "
        << countTotal << " (" << pourcentage << " %)\n";
}


// ── Affichage du résultat d'un trajet ────────────────────────────────────────
void afficherResultat(const Trajet& t, Critere critere, TypeTransport typePrefere)
{
    if (t.chemin == "Aucun trajet disponible" ||
        t.chemin == "Aucun trajet disponible via cette station")
    {
        cout << "  X " << t.chemin << "\n";
        return;
    }

    cout << "  Chemin    : " << t.chemin << "\n";
    switch (critere)
    {
    case Critere::TEMPS:
        cout << "  Duree     : " << t.data << " minutes\n";
        break;
    case Critere::OCCUPANCY:
        cout << "  Passagers : " << t.data << " (total sur le trajet)\n";
        break;
    case Critere::STATIONS:
        cout << "  Arrets    : " << t.data << " segments\n";
        break;
    }

    // Bilan du type de transport
    if (typePrefere != TypeTransport::TOUS)
    {
        if (t.hors_type == 0)
            cout << "  Type      : 100% " << typeToString(typePrefere) << " \n";
        else
            cout << "  ⚠ Trajet majoritairement en " << typeToString(typePrefere)
            << " (" << t.hors_type << " segment(s) avec un autre moyen de transport)\n";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Affectation optimale véhicules → lignes  (profil Exploitant, option 7)
// ─────────────────────────────────────────────────────────────────────────────

/*
 * affecterVehicules
 * ─────────────────
 * 1. Collecte toutes les liaisons actives (existe && !coupe) → liste de lignes
 * 2. Demande à l'utilisateur combien de véhicules il veut affecter
 * 3. Construit la matrice de coût via HungarianSolver::buildCostMatrix
 * 4. Résout avec HungarianSolver::solve
 * 5. Affiche le résultat : Véhicule k → Ligne (stationA → stationB)
 */
 void affecterVehicules(const vector<vector<Arc>>& mat,
    const vector<Station>& stations)
{
    int N = (int)mat.size();

    // ── 1. Collecter les lignes actives ───────────────────────────────────────
    vector<pair<int, int>> lignes;  // (from, to)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j].existe && !mat[i][j].coupe)
                lignes.push_back({ i, j });

    if (lignes.empty())
    {
        cout << "  Aucune ligne active dans le reseau.\n";
        return;
    }

    cout << "\n  " << lignes.size() << " lignes actives trouvees.\n";

    // ── 2. Nombre de véhicules ────────────────────────────────────────────────
    int maxVeh = min((int)lignes.size(), 20);   // plafonné pour la saisie
    int nbVeh = saisirEntier(
        "  Combien de vehicules a affecter ? (1-" + to_string(maxVeh) + ") : ",
        1, maxVeh);

    // ── 3. Matrice de coût ────────────────────────────────────────────────────
    vector<vector<float>> costMat =
        HungarianSolver::buildCostMatrix(mat, lignes, nbVeh);

    // ── 4. Résolution ─────────────────────────────────────────────────────────
    vector<int> affectation = HungarianSolver::solve(costMat);
    float       coutTotal = HungarianSolver::totalCost(costMat, affectation);

    // ── 5. Affichage ──────────────────────────────────────────────────────────
    cout << "\n  === Affectation optimale (coût total = "
        << coutTotal << ") ===\n";

    for (int i = 0; i < nbVeh; i++)
    {
        int j = affectation[i];
        if (j < 0 || j >= (int)lignes.size())
        {
            cout << "  Vehicule " << i + 1 << " : non affecte\n";
            continue;
        }

        int from = lignes[j].first;
        int to = lignes[j].second;

        cout << "  Vehicule " << i + 1
            << "  ->  Ligne  " << stations[from].nom
            << " --> " << stations[to].nom
            << "  (temps=" << mat[from][to].temps
            << "mn, occ=" << mat[from][to].occupancy << ")\n";
    }

    // ── Conseil : lignes non couvertes ────────────────────────────────────────
    // (utile si nbVeh < lignes.size())
    if (nbVeh < (int)lignes.size())
    {
        // Marquer les lignes affectées
        vector<bool> couverte(lignes.size(), false);
        for (int i = 0; i < nbVeh; i++)
            if (affectation[i] >= 0 && affectation[i] < (int)lignes.size())
                couverte[affectation[i]] = true;

        // Identifier les lignes à fort trafic non couvertes
        int maxOcc = 0, ligneMax = -1;
        for (int j = 0; j < (int)lignes.size(); j++)
        {
            if (!couverte[j] && mat[lignes[j].first][lignes[j].second].occupancy > maxOcc)
            {
                maxOcc = mat[lignes[j].first][lignes[j].second].occupancy;
                ligneMax = j;
            }
        }
        if (ligneMax != -1)
        {
            cout << "\n  Recommandation : la ligne la plus surchargee non couverte est\n";
            cout << "    " << stations[lignes[ligneMax].first].nom
                << " --> " << stations[lignes[ligneMax].second].nom
                << "  (occupancy=" << maxOcc << ")\n";
            cout << "  Envisagez d'augmenter le nombre de vehicules.\n";
        }
    }
}