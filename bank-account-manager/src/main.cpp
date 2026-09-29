#include <iostream>
#include <string>
#include "compte.h"
using namespace std;

int main() {

    int numero_compte = 1;
    string numero;

    // Charger les comptes depuis le fichier
    Noeud* debut = chargerComptesDepuisFichier("data.txt", numero_compte);

    if (debut == nullptr) {
        cout << "Aucun compte chargé (fichier vide ou créé)." << endl;
    }

    // Demander si l'utilisateur est un admin ou un client
    int role;
    cout << "Bienvenue. etes-vous :\n";
    cout << "1. Administrateur\n";
    cout << "2. Client\n";
    cout << "Entrez votre choix : ";
    cin >> role;

    if (role == 1 && estAdmin()) {
        // Si l'utilisateur est admin, afficher le menu admin
        menuAdmin(debut, numero_compte);
    }
    else if (role == 2) {
        
        string nom_client;
        cout << "Entrez votre nom ou prenom : ";
        cin >> nom_client;

        // Si l'utilisateur est un client, afficher le menu client en fonction du nom
        menuClient(debut, nom_client);
    }
    else {
        cout << "Accès refusé ou choix invalide." << endl;
    }

    // Sauvegarder les comptes avant de quitter
    sauvegarderComptesDansFichier(debut, "data.txt");
    supprimerTout(debut);  // Libérer la mémoire
    // Libérer la mémoire
    while (debut != nullptr)
    {
        Noeud* temp = debut;
        debut = debut->adsuivant;
        delete temp;
    }

    cout << "Au revoir !" << endl;
    return 0;
}