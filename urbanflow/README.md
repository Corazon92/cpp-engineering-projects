# UrbanFlow

Application console C++ de modélisation d'un réseau de transport public, réalisée en binôme.

## Fonctionnalités

- chargement des stations, connexions et coupures depuis des fichiers CSV ;
- graphe orienté représenté par une matrice d'adjacence enrichie ;
- calcul d'itinéraires avec Dijkstra selon le temps, l'occupation ou le nombre d'arrêts ;
- préférence de mode de transport et passage par une station intermédiaire ;
- prise en compte des coupures actives à une date donnée ;
- statistiques d'exploitation du réseau ;
- affectation de véhicules avec l'algorithme hongrois.

## Structure

```text
urbanflow/
├── src/
│   ├── main.cpp
│   ├── mesfonctions.cpp
│   ├── mesfonctions.h
│   ├── Hungarian.cpp
│   └── hungarian.h
└── data/
    ├── stations_project2.csv
    ├── connections_project2.csv
    └── coupures_lignes.csv
```

Le programme recherche les CSV dans son répertoire de travail. Pour l'exécuter, placez les trois fichiers de `data/` à côté de l'exécutable ou lancez l'exécutable depuis ce dossier.

Le code historique utilise `sscanf_s` et cible donc directement MSVC/Visual Studio. Les artefacts de compilation et chemins propres à l'ancien poste ne sont pas inclus.
