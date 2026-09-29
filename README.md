# C++ Engineering Projects

> Projet principal : **UrbanFlow**, modélisation d'un réseau de transport et recherche d'itinéraires.  
> **English version below.**

## 🇫🇷 UrbanFlow

Projet réalisé en binôme. Le programme charge trois fichiers CSV — stations, connexions et coupures — puis construit un graphe orienté sous forme de **matrice d'adjacence enrichie**.

Chaque arc conserve plusieurs propriétés : existence de la liaison, temps de trajet, occupation, coupure éventuelle et mode de transport.

## Recherche d'itinéraires

Le moteur utilise **Dijkstra** avec un poids adapté au critère demandé :
- temps de trajet ;
- occupation ;
- nombre d'arrêts.

Il gère aussi :
- une station intermédiaire imposée ;
- les coupures actives à la date choisie ;
- une préférence métro, bus ou tram.

Pour une préférence de transport, une première recherche tente d'utiliser uniquement le mode demandé. Si aucun chemin n'existe, une seconde recherche pénalise les autres modes plutôt que de rendre le trajet impossible.

La reconstruction du chemin utilise un tableau de prédécesseurs.

## Analyse exploitant

Le second profil du programme fournit :
- station la plus fréquentée ;
- connexion la plus utilisée ;
- nombre de coupures actives ;
- temps moyen entre stations ;
- normalisation/analyse de l'occupation ;
- affectation de véhicules avec l'**algorithme hongrois**.

## Architecture documentée

```text
main.cpp
├── chargement des données
├── choix de la date
├── menu usager
└── menu exploitant

mesfonctions.cpp / .h
├── génération de la matrice
├── Dijkstra
├── Dijkstra pénalisé
├── trajet via station
└── analyse du réseau

hungarian.cpp / .h
└── affectation / optimisation
```

## Ma contribution

La répartition indiquée dans le rapport est la suivante :
- **Samy Hammadi : Dijkstra, menu exploitant et fonctions d'analyse** ;
- Mohamed Bensafi : chargement CSV, structures et génération de la matrice ;
- intégration de l'algorithme hongrois et optimisation : travail commun.

## Limites identifiées

Le rapport relève trois limites principales : données CSV statiques, interface console et coût mémoire d'une matrice N×N pour un grand réseau peu dense. Une liste d'adjacence et des données temps réel seraient des évolutions naturelles.

## Sources

Le rapport technique décrit les structures et fonctions avec précision, mais les fichiers C++ finaux ne sont plus présents dans l'archive. Je ne publie donc pas une reconstitution présentée comme le code original.

---

# 🇬🇧 UrbanFlow

UrbanFlow is a two-person C++ project that models a public-transport network from station, connection and disruption CSV files.

The network is represented as an enriched directed adjacency matrix. Route computation uses **Dijkstra's algorithm** with different edge costs for travel time, occupancy and number of stops. It also supports intermediate stations, date-dependent disruptions and transport-mode preferences.

An operator menu computes network indicators and uses the **Hungarian algorithm** for vehicle allocation.

### My contribution

According to the project report, my main work covered **Dijkstra route computation, the operator menu and network-analysis functions**. CSV loading and matrix generation were handled by my teammate; Hungarian-algorithm integration was joint work.

The final C++ files are no longer available in the archive, so this repository documents the verified implementation without fabricating source code.
