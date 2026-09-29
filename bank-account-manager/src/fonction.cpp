#include <iostream>
#include <string>
#include <cstdio>
#include <regex>
#include <algorithm> // pour transform()
#include"compte.h"
using namespace std;

void toLower(string& str) 
{
    // Convertir une chaîne en minuscules
    transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int obtenirMaxNumeroCompte(Noeud* debut)
{
    int maxNumero = 0;
    Noeud* courant = debut;

    while (courant != nullptr) 
    {
        int numeroCompte = std::stoi(courant->compte.Num_compte);
        if (numeroCompte > maxNumero) {
            maxNumero = numeroCompte;
        }
        courant = courant->adsuivant;
    }
    return maxNumero;
}
bool estUnEntier(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) { // Vérifie si chaque caractère est un chiffre
            return false;
        }
    }
    return true;
}
void saisirCompte(Noeud*& debut, int& numero_compte) {
    Noeud* adn = new Noeud;

    // Saisie et validation du nom
    while (true) {
        cout << "Entrez le nom : ";
        cin >> adn->compte.nom;

        // Vérifier que le nom contient uniquement des lettres
        if (regex_match(adn->compte.nom, regex("^[A-Za-z]+$"))) {
            break;
        }
        else {
            cout << "Erreur : Le nom ne doit contenir que des lettres. Veuillez reessayer." << endl;
        }
    }

    // Saisie et validation du prenom
    while (true) {
        cout << "Entrez le prenom : ";
        cin >> adn->compte.prenom;

        // Vérifier que le prenom contient uniquement des lettres
        if (regex_match(adn->compte.prenom, regex("^[A-Za-z]+$"))) {
            break;
        }
        else {
            cout << "Erreur : Le prenom ne doit contenir que des lettres. Veuillez reessayer." << endl;
        }
    }

    // Génération du numéro de compte
    adn->compte.Num_compte = to_string(numero_compte++);

    // Saisie et validation du code (doit être un entier de 4 chiffres)
    string codeStr;
    bool codeValide = false;

    // Boucle pour obtenir un code valide
    while (!codeValide) {
        cout << "Entrez un code a 4 chiffres : ";
        cin >> codeStr;

        // Vérifie que le code a exactement 4 chiffres et qu'il est un entier
        if (codeStr.length() == 4 && estUnEntier(codeStr)) {
            adn->compte.code = std::stoi(codeStr); // Convertir la chaîne en entier
            codeValide = true; // Code valide, sortir de la boucle
        }
        else {
            cout << "Erreur : Veuillez entrer un code compose de 4 chiffres." << endl;
        }
    }

    // Saisie du solde
    cout << "Entrez le solde : ";
    cin >> adn->compte.solde;

    // Insertion du nouveau noeud dans la liste
    adn->adsuivant = debut;
    debut = adn;
}

void saisir100Compte(Noeud*& debut, int& numero_compte) {
    Noeud* adn = new Noeud;
    adn->compte.nom = "Hammadi";
    adn->compte.prenom = " Samy";
    // Convertir l'int en string pour le numéro de compte
    adn->compte.Num_compte = to_string(numero_compte++);
    adn->compte.code = 1000;
    adn->compte.solde = 10;
    adn->adsuivant = debut;
    debut = adn;
}
// Fonction pour afficher un compte
void afficherCompte(Noeud* n) {
    if (n == nullptr) { // Vérification si le nœud est valide
        cout << "Erreur : Le pointeur n est nul." << endl;
        return;
    }

    cout << "Nom : " << n->compte.nom << endl;
    cout << "Prenom : " << n->compte.prenom << endl;
    cout << "Numero de compte : " << n->compte.Num_compte << endl;
    cout << "Solde : " << n->compte.solde << endl;
    cout << "-----------" << endl;
}
void afficherTousComptes(Noeud* debut) {
    if (debut == nullptr) { // Vérification si la liste est vide
        cout << "Aucun compte a afficher." << endl;
        return; // Sortir de la fonction si la liste est vide
    }

    Noeud* adc = debut;
    while (adc != nullptr) {
        afficherCompte(adc);  // Utilisation de la fonction afficherCompte
        adc = adc->adsuivant; // Passer au nœud suivant
    }
}
void afficher1Compte(Noeud* debut, const string& numero_compte) {
    Noeud* courant = debut;
    int rep;
    int mdp;
    int nombre = 3;  // Nombre de tentatives autorisées

    // Parcours de la liste pour trouver le compte
    while (courant != nullptr) {
        // Si le numéro de compte correspond
        if (courant->compte.Num_compte == numero_compte) {
            // Afficher les détails du compte
            cout << "Details du compte :" << endl;
            cout << "Nom : " << courant->compte.nom << endl;
            cout << "Prenom : " << courant->compte.prenom << endl;
            cout << "Numero de compte : " << courant->compte.Num_compte << endl;
            cout << "Solde : " << courant->compte.solde << endl;

            // Demander à l'utilisateur s'il veut afficher le code
            cout << "Voulez-vous afficher le code du compte ? (Mot de passe requis) (1 pour oui / 0 pour non)" << endl;
            cin >> rep;

            if (rep == 1) {
                while (nombre > 0) {
                    cout << "Entrez le mot de passe administrateur : " << endl;
                    cin >> mdp;

                    if (mdp == 0) {  // Mot de passe correct
                        cout << "Code : " << courant->compte.code << endl;
                        return;  // Sortir de la fonction une fois le code affiché
                    }
                    else {
                        nombre--;
                        if (nombre > 0) {
                            cout << "Mauvais mot de passe. Il vous reste " << nombre << " tentative(s)." << endl;
                        }
                        else {
                            cout << "Nombre de tentatives depasse. Acces refuse." << endl;
                        }
                    }
                }
            }

            return; // Sortir de la fonction après avoir affiché les détails
        }

        courant = courant->adsuivant; // Avancer au nœud suivant
    }

    // Si le compte n'est pas trouvé
    cout << "Compte avec le numero " << numero_compte << " introuvable." << endl;
}

// Fonction pour supprimer un compte à l'aide du numéro de compte
void supprimerCompte(Noeud*& debut, const string& numero_compte) // *& = passage par référence
{
    Noeud* courant = debut;
    Noeud* precedent = nullptr;

    // Parcours de la liste pour trouver le nœud à supprimer
    while (courant != nullptr) {
        // Si le numéro de compte correspond
        if (courant->compte.Num_compte == numero_compte) {
            // Afficher les détails du compte avant suppression
            cout << "Details du compte a supprimer :" << endl;
            afficher1Compte(debut, numero_compte);

            // Demander la confirmation à l'utilisateur
            char confirmation;
            cout << "Etes-vous sur de vouloir supprimer ce compte ? (o/n) : ";
            cin >> confirmation;

            if (confirmation == 'o' || confirmation == 'O') {
                if (precedent == nullptr) {
                    // Si le nœud à supprimer est le premier de la liste
                    debut = courant->adsuivant; // On décale le début avant de supprimer
                }
                else {
                    // Sinon, relier le nœud précédent au suivant
                    precedent->adsuivant = courant->adsuivant; // le précédent pointe sur 2 devant
                }
                delete courant;  // Libérer la mémoire du nœud supprimé
                cout << "Compte avec le numero " << numero_compte << " a ete supprime." << endl;
            }
            else {
                cout << "Suppression annulee." << endl;
            }
            return;
        }
        precedent = courant; // on avance d'un
        courant = courant->adsuivant; // on avance d'un
    }

    // Si le compte n'est pas trouvé
    cout << "Compte avec le numero " << numero_compte << " introuvable." << endl;
}
void supprimerDebut(Noeud*& debut)
{
    Noeud* courant;
    Noeud* suivant;
    courant = debut;
    debut = courant->adsuivant;
    suivant = courant->adsuivant;
    delete courant;
    courant = suivant;

}

void supprimerTout(Noeud*& debut)
{
    Noeud* courant;
    Noeud* suivant;
    courant = debut;

    while (courant != nullptr) // on vérifie que courant n'est pas nulle 
    {
        debut = courant->adsuivant;
        suivant = courant->adsuivant;
        delete courant;
        courant = suivant;
    }
}
void modifierSolde(Noeud* debut, const string& numero_compte) {
    Noeud* courant = debut;

    // Parcours de la liste pour trouver le nœud à modifier
    while (courant != nullptr) {
        // Si le numéro de compte correspond
        if (courant->compte.Num_compte == numero_compte) {
            cout << "Modification du compte avec le numero : " << numero_compte << endl;
            afficher1Compte(debut, numero_compte);
            // Affiche le solde actuel
            cout << "Solde actuel : " << courant->compte.solde << " euros. " << endl;

            // Vérification du code pour modifier le solde
            int essaisRestants = 3;
            int codeSaisi;

            while (essaisRestants > 0) {
                cout << "Veuillez entrer le code pour acceder a la modification du solde : ";
                cin >> codeSaisi;

                if (codeSaisi == courant->compte.code) {
                    cout << "Code correct." << endl;

                    int action;
                    double montant;

                    // Choix entre ajouter ou retirer du solde
                    cout << "Que souhaitez-vous faire ?" << endl;
                    cout << "1. Ajouter au solde" << endl;
                    cout << "2. Retirer du solde" << endl;
                    cout << "Entrez votre choix : ";
                    cin >> action;

                    if (action == 1) {
                        cout << "Entrez le montant a ajouter : ";
                        cin >> montant;
                        courant->compte.solde += montant;
                        cout << "Nouveau solde : " << courant->compte.solde << " euros." << endl;
                    }
                    else if (action == 2) {
                        cout << "Entrez le montant a retirer : ";
                        cin >> montant;

                        if (montant <= courant->compte.solde) {
                            courant->compte.solde -= montant;
                            cout << "Nouveau solde : " << courant->compte.solde << " euros." << endl;
                        }
                        else {
                            cout << "Montant insuffisant sur le compte." << endl;
                        }
                    }
                    else {
                        cout << "Choix invalide." << endl;
                    }
                    return;
                }
                else {
                    essaisRestants--;
                    cout << "Code incorrect. Il vous reste " << essaisRestants << " tentative(s)." << endl;
                }

                if (essaisRestants == 0) {
                    cout << "Acces refuse après 3 tentatives incorrectes." << endl;
                    return;
                }
            }
        }

        // Passe au nœud suivant
        courant = courant->adsuivant;
    }

    // Si le compte n'est pas trouvé
    cout << "Compte avec le numero " << numero_compte << " introuvable." << endl;
}
void modifierCompte(Noeud* debut, const string& numero_compte) 
{
    Noeud* courant = debut;

    // Parcours de la liste pour trouver le nœud à modifier
    while (courant != nullptr) {
        // Si le numéro de compte correspond
        if (courant->compte.Num_compte == numero_compte) {
            cout << "Modification du compte avec le numero : " << numero_compte << endl;
            afficher1Compte(debut, numero_compte);
            // Vérification du mot de passe administrateur avant modification
            int essaisRestants = 3;
            int mdpAdmin;
            while (essaisRestants > 0) {
                cout << "Veuillez entrer le mot de passe administrateur pour acceder à la modification : ";
                cin >> mdpAdmin;
                
                if (mdpAdmin == 0) { // code de démonstration uniquement
                    cout << "Acces administrateur accorde." << endl;

                    int choix;
                    string nouveauNom, nouveauPrenom;
                    string codeStr; // Utiliser une chaîne pour vérifier le code d'accès
                    bool codeValide = false;

                    // Choix entre modifier le nom, le prénom ou le code
                    cout << "Que souhaitez-vous modifier ?" << endl;
                    cout << "1. Nom" << endl;
                    cout << "2. Prenom" << endl;
                    cout << "3. Code d'acces" << endl;
                    cout << "4. Quitter sans modification" << endl;
                    cout << "Entrez votre choix : ";
                    cin >> choix;

                    switch (choix) {
                    case 1:
                        while (true) {
                            cout << "Entrez le nom : ";
                            cin >> nouveauNom;

                            // Vérifier que le nom contient uniquement des lettres
                            if (regex_match(nouveauNom, regex("^[A-Za-z]+$"))) {
                                courant->compte.nom = nouveauNom;
                                cout << "Le nom a ete modifie." << endl;
                                break;
                            }
                            else {
                                cout << "Erreur : Le nom ne doit contenir que des lettres. Veuillez reessayer." << endl;
                            }
                        }
                        break;

                    case 2:
                        while (true) {
                            cout << "Entrez le prenom : ";
                            cin >> nouveauPrenom;

                            // Vérifier que le prénom contient uniquement des lettres
                            if (regex_match(nouveauPrenom, regex("^[A-Za-z]+$"))) {
                                courant->compte.prenom = nouveauPrenom;
                                cout << "Le prenom a ete modifie." << endl;
                                break;
                            }
                            else {
                                cout << "Erreur : Le prenom ne doit contenir que des lettres. Veuillez reessayer." << endl;
                            }
                        }
                        break;

                    case 3:
                        // Boucle pour obtenir un code valide
                        while (!codeValide) {
                            cout << "Entrez un code à 4 chiffres : ";
                            cin >> codeStr;

                            // Vérifie que le code a exactement 4 chiffres et qu'il est numérique
                            if (codeStr.length() == 4 && estUnEntier(codeStr)) {
                                courant->compte.code = std::stoi(codeStr); // Convertir la chaîne en entier
                                codeValide = true; // Code valide, sortir de la boucle
                                cout << "Le code d'acces a ete modifie." << endl;
                            }
                            else {
                                cout << "Erreur : Veuillez entrer un code compose de 4 chiffres." << endl;
                            }
                        }
                        break;

                    case 4:
                        cout << "Aucune modification effectuee." << endl;
                        return;

                    default:
                        cout << "Choix invalide." << endl;
                        break;
                    }
                    return;
                }
                else {
                    essaisRestants--;
                    cout << "Mot de passe incorrect. Il vous reste " << essaisRestants << " tentative(s)." << endl;
                }

                if (essaisRestants == 0) {
                    cout << "Acces refuse apres 3 tentatives incorrectes." << endl;
                    return;
                }
            }
        }

        // Passe au nœud suivant
        courant = courant->adsuivant;
    }

    // Si le compte n'est pas trouvé
    cout << "Compte avec le numero " << numero_compte << " introuvable." << endl;
}

void sauvegarderComptesDansFichier(Noeud* debut, const string& nomFichier) {
    FILE* fichier;

    if (fopen_s(&fichier, nomFichier.c_str(), "w") == 0) { // Ouvrir en mode écriture
        Noeud* courant = debut;

        while (courant != nullptr) {
            // Conversion du code en chaîne de caractères pour le crypter
            char codeString[5];
            sprintf_s(codeString, "%04d", courant->compte.code); // Convertir l'int en string avec 4 digits

            // Crypter le code
            char codeCrypte[5];
            code(codeString, codeCrypte);

            // Enregistrer sous le format Nom_Prenom_NumCompte_CodeCrypté_Solde
            fprintf(fichier, "%s %s %s %s %.2f\n",
                courant->compte.nom.c_str(),
                courant->compte.prenom.c_str(),
                courant->compte.Num_compte.c_str(),
                codeCrypte,  // Enregistrer le code crypté
                courant->compte.solde);

            courant = courant->adsuivant;
        }

        _fcloseall(); // Fermer le fichier
    }
    else {
        cout << "Erreur lors de l'ouverture du fichier pour écriture." << endl;
    }
}
/// Fonction pour charger les comptes depuis un fichier texte
Noeud* chargerComptesDepuisFichier(const string& nomFichier, int& numero_compte) {
    FILE* fichier;

    // Essayer d'ouvrir le fichier en mode lecture
    if (fopen_s(&fichier, nomFichier.c_str(), "r") != 0) {
        // Créer le fichier en mode écriture s'il n'existe pas
        if (fopen_s(&fichier, nomFichier.c_str(), "w") != 0) {
            cout << "Erreur lors de la création du fichier." << endl;
            return nullptr;
        }
        _fcloseall(); // Fermer le fichier créé
        return nullptr;
    }

    char buffer[256];
    Noeud* debut = nullptr;
    int code;
    double solde;

    // Lire les données depuis le fichier
    char nomTemp[100], prenomTemp[100], numCompteTemp[100], codeCrypte[5];
    while (fscanf_s(fichier, "%99s %99s %99s %4s %lf",
        nomTemp, sizeof(nomTemp),
        prenomTemp, sizeof(prenomTemp),
        numCompteTemp, sizeof(numCompteTemp),
        codeCrypte, sizeof(codeCrypte),
        &solde) == 5)
    {
        // Décryptage du code
        char codeDecrypte[5];
        for (int i = 0; i < 4; i++) {
            codeDecrypte[i] = codeCrypte[i] ^ 'z';  // XOR pour décrypter
        }
        codeDecrypte[4] = '\0';  // Terminer la chaîne de caractères

        // Convertir le code décrypté en entier
        code = atoi(codeDecrypte);

        // Création d'un nouveau noeud
        Noeud* nouveau = new Noeud;
        nouveau->compte.nom = string(nomTemp);  // Convertir en string (copie)
        nouveau->compte.prenom = string(prenomTemp);  // Convertir en string (copie)
        nouveau->compte.Num_compte = numCompteTemp;  // Générer le numéro de compte
        numero_compte++;
        nouveau->compte.code = code;
        nouveau->compte.solde = solde;

        // Ajout du nouveau noeud au début de la liste chaînée
        nouveau->adsuivant = debut;
        debut = nouveau;
    }

    fclose(fichier); // Fermer le fichier après la lecture
    return debut; // Retourner la liste chaînée
}
void numbersToAscii(int entier, char asciiString[])
{
    int n = entier, r;
    for (int i = 3; i >= 0; i--)
    {
        r = n % 10;
        n = n / 10;
        asciiString[i] = r + '0';
    }
    asciiString[4] = '\0';
}
void code(char asciiString[], char asciicode[])
{
    for (int i = 0; i < 4; i++)
    {
        asciicode[i] = asciiString[i] ^ 'z';

    }
    asciicode[4] = '\0';

}
void rechercherCompte(Noeud* debut, const string& nom_client, vector<string>& comptesClient) {
    string recherche = nom_client;
    toLower(recherche);

    Noeud* courant = debut;
    bool compteTrouve = false;

    cout << "Resultats de la recherche pour '" << recherche << "':" << endl;

    while (courant != nullptr) {
        string nomMin = courant->compte.nom;
        string prenomMin = courant->compte.prenom;
        toLower(nomMin);
        toLower(prenomMin);

        if (nomMin == recherche || prenomMin == recherche) {
            afficherCompte(courant);
            comptesClient.push_back(courant->compte.Num_compte);  // Ajoute le numéro de compte
            compteTrouve = true;
        }
        courant = courant->adsuivant;
    }

    if (!compteTrouve) {
        cout << "Aucun compte trouve avec ce nom ou prenom." << endl;
    }
}

// Ajoute une fonction pour vérifier si l'utilisateur est admin ou client
bool estAdmin() {
    int motDePasse;
    cout << "Entrez le mot de passe administrateur (1234) : ";
    cin >> motDePasse;
    return motDePasse == 1234;
}

// Fonction pour les actions admin
void menuAdmin(Noeud*& debut, int& numero_compte) {
    int rep_utilisateur = 1;
    string numero;

    while (rep_utilisateur == 1) {
        cout << "\n--- Menu Admin ---" << endl;
        cout << "1. Ajouter un compte\n";
        cout << "2. Modifier un compte\n";
        cout << "3. Afficher tous les comptes\n";
        cout << "4. Supprimer un compte\n";
        cout << "5. Afficher un compte\n";
        cout << "6. Quitter\n";
        cout << "Entrez votre choix : ";
        int choix;
        cin >> choix;

        switch (choix) {
        case 1:
            cout << " ---Ajouter un Compte--- " << endl;
            saisirCompte(debut, numero_compte);
            break;
        case 2:
            cout << " ---Modifier un Compte--- " << endl;
            cout << "Entrez le numero de compte a modifier : ";
            cin >> numero;
            modifierCompte(debut, numero);
            break;
        case 3:
            cout << " ---Afficher tous les Comptes--- " << endl;
            afficherTousComptes(debut);
            break;
        case 4:
            cout << " --Supprimer un Compte--- " << endl;
            cout << "Entrez le numero de compte a supprimer : ";
            cin >> numero;
            supprimerCompte(debut, numero);
            break;
        case 5:
            cout << " ---Afficher un Compte--- " << endl;
            cout << "Entrez le numero de compte a afficher : ";
            cin >> numero;
            afficher1Compte(debut, numero);
            break;
        case 6:
            rep_utilisateur = 0; // Quitter
            break;
        default:
            cout << "Choix invalide." << endl;
            break;
        }
    }
}

// Fonction pour les actions client
void menuClient(Noeud* debut, const string& nom_client) {
    int rep_utilisateur = 1;
    vector<string> comptesClient;  // Stocke les numéros de compte associés au client
    rechercherCompte(debut, nom_client, comptesClient);  // Recherche et remplit la liste des comptes

    while (rep_utilisateur == 1) {
        cout << "\n--- Menu Client ---" << endl;
        cout << "1. Modifier le solde d'un de vos comptes\n";
        cout << "2. Afficher tous vos comptes\n";
        cout << "3. Quitter\n";
        cout << "Entrez votre choix : ";
        int choix;
        cin >> choix;

        switch (choix) {
        case 1:
            if (comptesClient.empty()) {
                cout << "Vous n'avez aucun compte à modifier." << endl;
                break;
            }

            cout << " ---Modifier le solde--- " << endl;
            {
                string numCompte;
                cout << "Entrez le numero de compte que vous souhaitez modifier : ";
                cin >> numCompte;

                // Vérification du numéro de compte
                if (find(comptesClient.begin(), comptesClient.end(), numCompte) != comptesClient.end()) {
                    modifierSolde(debut, numCompte);  // Numéro de compte valide pour ce client
                }
                else {
                    cout << "Erreur : Ce numero de compte ne vous appartient pas." << endl;
                }
            }
            break;

        case 2:
            cout << "-- Afficher vos comptes---" << endl;
            rechercherCompte(debut, nom_client,comptesClient);  // Affiche tous les comptes du client
            break;

        case 3:
            rep_utilisateur = 0; // Quitter
            break;

        default:
            cout << "Choix invalide." << endl;
            break;
        }
    }
}