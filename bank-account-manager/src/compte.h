#include <iostream>
#include <string>
using namespace std;

// Déclaration du type composé "Compte"
struct Compte {
    string nom;
    string prenom;
    string Num_compte;
    int code;
    double solde;
};
struct Noeud {
    Compte compte;         // Le compte
    Noeud* adsuivant;      // Pointeur vers le nœud suivant
};
bool estUnEntier(const std::string& str);
void saisirCompte(Noeud*& debut, int& numero_compte);
void afficherCompte(Noeud* n);
void afficherTousComptes(Noeud* debut);
void supprimerCompte(Noeud*& debut, const string& numero_compte);
void supprimerDebut(Noeud*& debut);
void supprimerTout(Noeud*& debut);
void saisir100Compte(Noeud*& debut, int& numero_compte);
// Fonction pour modifier un compte à partir de son numéro
void modifierSolde(Noeud* debut, const string& numero_compte);
void modifierCompte(Noeud* debut, const string& numero_compte);
// Fonction pour enregistrer les comptes dans un fichier texte 
void sauvegarderComptesDansFichier(Noeud* debut, const string& nomFichier);
Noeud* chargerComptesDepuisFichier(const string& nomFichier, int& numero_compte);
void afficher1Compte(Noeud* debut, const string& numero_compte);
void numbersToAscii(int entier, char asciiString[]);
void code(char asciiString[], char asciicode[]);
bool estAdmin();
void menuAdmin(Noeud*& debut, int& numero_compte);
void menuClient(Noeud* debut, const string& nom_client);
void rechercherCompte(Noeud* debut);
void toLower(string& str);
int obtenirMaxNumeroCompte(Noeud* debut);