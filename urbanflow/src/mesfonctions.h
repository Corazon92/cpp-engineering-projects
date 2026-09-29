#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// ─────────────────────────────────────────────
//  Structures de données
// ─────────────────────────────────────────────

struct Date {
    int jour = 0, mois = 0, annee = 0;

    bool operator>=(const Date& o) const {
        if (annee != o.annee) return annee >= o.annee;
        if (mois != o.mois)  return mois >= o.mois;
        return jour >= o.jour;
    }
    bool operator<=(const Date& o) const {
        if (annee != o.annee) return annee <= o.annee;
        if (mois != o.mois)  return mois <= o.mois;
        return jour <= o.jour;
    }
};

struct Station {
    string id;
    string nom;
};

struct Connection {
    string from, to;
    string temps;
    string type;
    string frequence;
    string occupancy;
};

struct Coupure {
    string from, to;
    Date   date_debut, date_fin;
};

/*
 * TypeTransport : type de véhicule sur une liaison.
 * TOUS est utilisé quand l'usager n'a pas de préférence.
 */
enum class TypeTransport { METRO, BUS, TRAM, TOUS };

// Conversion string CSV → enum
TypeTransport stringToType(const string& s);
// Conversion enum → label affichable
string        typeToString(TypeTransport t);

/*
 * Arc : regroupe toutes les informations d'une liaison i→j.
 *   existe    : il y a bien une liaison
 *   temps     : durée en minutes
 *   occupancy : passagers moyens
 *   coupe     : liaison interrompue à la date du trajet
 *   type      : metro / bus / tram
 */
struct Arc {
    bool          existe = false;
    int           temps = 0;
    int           occupancy = 0;
    bool          coupe = false;
    TypeTransport type = TypeTransport::BUS;
};

/*
 * Trajet : résultat d'un calcul de chemin.
 *   chemin      : stations séparées par " -> "
 *   data        : valeur minimisée (minutes, passagers, ou nb segments)
 *   hors_type   : nombre de segments n'utilisant pas le type préféré
 *                 (0 si pas de préférence ou trajet 100% conforme)
 */
struct Trajet {
    string chemin;
    int    data = 0;
    int    hors_type = 0;
};

/*
 * Critere : détermine quel poids Dijkstra minimise.
 *   TEMPS      → a.temps
 *   OCCUPANCY  → a.occupancy
 *   STATIONS   → 1 (nombre de sauts)
 */
enum class Critere { TEMPS, OCCUPANCY, STATIONS };

// ─────────────────────────────────────────────
//  Prototypes des fonctions
// ─────────────────────────────────────────────


//Fonctions utilitaires
Date              stringToDate(const string& str);
int saisirEntier(const string& message, int min, int max);
TypeTransport saisirType();
void afficherResultat(const Trajet& t, Critere critere, TypeTransport typePrefere);

//Fonctions Matrice
vector<Station>   charger_stations(const string& nomFichier);
vector<Connection>charger_connections(const string& nomFichier);
vector<Coupure>   charger_coupures(const string& nomFichier);

vector<vector<Arc>> generer_matrice_reseau(
    const vector<Station>& stations,
    const vector<Connection>& connections,
    const vector<Coupure>& coupures,
    Date                      date_trajet);

void afficherMat(const vector<vector<Arc>>& mat);

Trajet plusCourtTrajet(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int arrivee,
    Critere critere,
    TypeTransport typePrefere = TypeTransport::TOUS);

Trajet trajetViaStation(
    const vector<vector<Arc>>& mat,
    const vector<Station>& stations,
    int depart, int inter, int arrivee,
    Critere critere,
    TypeTransport typePrefere = TypeTransport::TOUS);

// Fonctions exploitant
void stationPlusFrequentee(const vector<vector<Arc>>& mat, const vector<Station>& stations);
void connexionPlusUtilisee(const vector<vector<Arc>>& mat, const vector<Station>& stations);
void nombreCoupures(const vector<vector<Arc>>& mat);
void tempsMoyen(const vector<vector<Arc>>& mat);
vector<vector<float>> normaliserOccupancy(const vector<vector<Arc>>& mat);
void analyserMatriceNormalisee(const vector<vector<float>>& matNorm,const vector<Station>& stations);
void affecterVehicules(const vector<vector<Arc>>& mat, const vector<Station>& stations);