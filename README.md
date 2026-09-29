# C++ Engineering Projects

> Collection de projets C++ : **algorithmique/optimisation** et **programmation consultable avec structures dynamiques**.  
> **English version below.**

## 🇫🇷 Projets

### 1. UrbanFlow — réseau de transport & optimisation

Projet réalisé en binôme autour d'un réseau de transport chargé depuis des fichiers CSV.

Le réseau est représenté par un graphe orienté sous forme de matrice d'adjacence enrichie. Le calcul d'itinéraire utilise **Dijkstra** avec différents critères : temps, occupation ou nombre d'arrêts. Le projet gère également une station intermédiaire, des coupures actives selon la date et une préférence de mode de transport.

Le menu exploitant propose notamment l'analyse de fréquentation et l'affectation de véhicules avec l'**algorithme hongrois**.

**Contribution documentée :** calcul d'itinéraires avec Dijkstra, menu exploitant et fonctions d'analyse ; intégration/optimisation de l'algorithme hongrois réalisée en commun.

**Disponibilité :** les sources C++ finales retrouvées sont désormais publiées dans [`urbanflow/`](urbanflow/), avec les jeux de données CSV nécessaires.

### 2. Bank Account Manager — listes chaînées & persistance

Application console C++ de gestion de comptes utilisant une **liste chaînée dynamique**.

Fonctionnalités vérifiées dans les sources :
- structures `Compte` et `Noeud` ;
- création, consultation, modification et suppression ;
- allocation/libération dynamique ;
- recherche de comptes ;
- validation des entrées ;
- rôles administrateur/client ;
- sauvegarde et rechargement depuis un fichier ;
- transformation/chiffrement pédagogique de données par XOR.

Les sources originales retrouvées sont disponibles dans :

`bank-account-manager/src/`

### Nettoyage de sécurité

L'ancien projet contenait un code administrateur écrit directement dans `fonction.cpp`. Cette valeur n'a **jamais été publiée dans ce dépôt** : elle a été remplacée avant le premier commit par une valeur de démonstration non secrète.

Le mécanisme d'authentification reste pédagogique et ne doit pas être considéré comme un système de sécurité destiné à la production.

---

# 🇬🇧 C++ Engineering Projects

This repository combines two complementary C++ projects.

### UrbanFlow

A public-transport network model using graph algorithms, **Dijkstra**, multiple route criteria and the **Hungarian algorithm** for vehicle allocation. The recovered final source files and CSV datasets are available in [`urbanflow/`](urbanflow/).

### Bank Account Manager

A console application whose recovered original sources demonstrate:
- structs and pointers;
- dynamic linked lists;
- CRUD operations;
- input validation;
- file persistence;
- administrator/client roles;
- a small educational XOR-based data transformation.

The historical code is intentionally kept close to its student-project form. A hard-coded administrator value found in the archive was sanitized **before the first public commit**.
