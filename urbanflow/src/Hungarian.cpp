#include "hungarian.h"
#include "mesfonctions.h"   // pour struct Arc

#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

// ─────────────────────────────────────────────────────────────────────────────
//  solve  –  algo hongrois (méthode des potentiels de Kuhn–Munkres)
//            complexité O(N³), fonctionne sur matrice carrée
// ─────────────────────────────────────────────────────────────────────────────
vector<int> HungarianSolver::solve(vector<vector<float>> cost)
{
    int origRows = (int)cost.size();
    if (origRows == 0) return {};

    // Rendre carrée
    int n = origRows;
    for (auto& row : cost)
        if ((int)row.size() > n) n = (int)row.size();
    for (auto& row : cost)
        row.resize(n, 1e9f);
    while ((int)cost.size() < n)
        cost.push_back(vector<float>(n, 1e9f));

    vector<float> u(n + 1, 0.f), v(n + 1, 0.f);
    vector<int>   p(n + 1, 0), way(n + 1, 0);

    for (int i = 1; i <= n; i++)
    {
        p[0] = i;
        int j0 = 0;
        vector<float> minVal(n + 1, 1e9f);
        vector<bool>  used(n + 1, false);

        do {
            used[j0] = true;
            int i0 = p[j0];
            int j1 = -1;
            float delta = 1e9f;

            for (int j = 1; j <= n; j++)
            {
                if (!used[j])
                {
                    float cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minVal[j]) { minVal[j] = cur; way[j] = j0; }
                    if (minVal[j] < delta) { delta = minVal[j]; j1 = j; }
                }
            }

            // ← CORRECTION : si aucune colonne libre trouvée, on arrête
            if (j1 == -1) break;

            for (int j = 0; j <= n; j++)
            {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; }
                else          minVal[j] -= delta;
            }

            j0 = j1;
        } while (p[j0] != 0);

        // Remontée du chemin augmentant — uniquement si j0 valide
        if (j0 > 0)
        {
            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0);
        }
    }

    // Reconstruction résultat
    vector<int> result(origRows, -1);
    for (int j = 1; j <= n; j++)
        if (p[j] != 0 && p[j] - 1 < origRows)
            result[p[j] - 1] = j - 1;

    return result;
}

// ─────────────────────────────────────────────────────────────────────────────
//  totalCost
// ─────────────────────────────────────────────────────────────────────────────
float HungarianSolver::totalCost(const vector<vector<float>>& mat,
    const vector<int>& assignment)
{
    float total = 0.f;
    for (int i = 0; i < (int)assignment.size(); i++)
    {
        int j = assignment[i];
        if (j >= 0 && j < (int)mat[i].size())
            total += mat[i][j];
    }
    return total;
}

// ─────────────────────────────────────────────────────────────────────────────
//  buildCostMatrix
//  Coût(véhicule i, ligne j) = temps_moyen(j) / max(1, occupancy_moyen(j))
//  → un véhicule rapide et peu chargé a un coût faible = affectation préférée
// ─────────────────────────────────────────────────────────────────────────────
vector<vector<float>> HungarianSolver::buildCostMatrix(
    const vector<vector<Arc>>& mat,
    const vector<pair<int, int>>& lignes,
    int                               nbVehicules)
{
    int nV = nbVehicules;
    int nL = (int)lignes.size();

    vector<vector<float>> cost(nV, vector<float>(nL, 1e9f));

    for (int j = 0; j < nL; j++)
    {
        int from = lignes[j].first;
        int to = lignes[j].second;

        // Vérifier que la liaison existe et n'est pas coupée
        if (!mat[from][to].existe || mat[from][to].coupe)
            continue;   // coût 1e9f → ne sera jamais choisi

        float temps = (float)mat[from][to].temps;
        float occupancy = (float)max(1, mat[from][to].occupancy);

        float c = temps / occupancy;  // faible = ligne intéressante

        // Tous les véhicules ont le même coût pour une ligne donnée
        // (à étendre si les véhicules ont des capacités différentes)
        for (int i = 0; i < nV; i++)
            cost[i][j] = c;
    }

    return cost;
}