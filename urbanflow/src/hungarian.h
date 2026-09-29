#pragma once

#include <vector>
#include <string>

/*
 * HungarianSolver
 * ───────────────
 * Résout le problème d'affectation optimale (minimisation du coût total)
 * sur une matrice N×M en O(N³).
 *
 * Cas d'usage dans UrbanFlow (profil Exploitant) :
 *   - Affecter N véhicules à M lignes de façon à minimiser
 *     le coût global (temps de trajet, occupancy, etc.)
 *   - La matrice de coût est construite depuis les données Arc
 *     de la matrice du réseau.
 *
 * Si N ≠ M, la matrice est complétée automatiquement (padding 1e9f)
 * pour devenir carrée — les affectations fictives sont ignorées.
 */
class HungarianSolver
{
public:
    /*
     * solve(costMatrix)
     * ─────────────────
     * Entrée  : costMatrix[i][j] = coût d'affecter la ressource i à la tâche j
     * Sortie  : assignment[i]    = indice j de la tâche assignée à la ressource i
     *           (-1 si la ressource est une ligne de padding)
     */
    static std::vector<int> solve(std::vector<std::vector<float>> costMatrix);

    /*
     * totalCost(costMatrix, assignment)
     * ───────────────────────────────────
     * Calcule le coût total réel de l'affectation.
     */
    static float totalCost(const std::vector<std::vector<float>>& mat,
        const std::vector<int>& assignment);

    /*
     * buildCostMatrix(mat, lignes, vehicules)
     * ────────────────────────────────────────
     * Construit une matrice de coût vehicules × lignes
     * depuis la matrice Arc du réseau.
     *
     * Coût d'affecter le véhicule i à la ligne j :
     *   = temps_moyen(ligne j) / max(1, occupancy_moyen(ligne j))
     *   → favorise les lignes rapides et peu bondées.
     *
     * Paramètres :
     *   mat         : matrice Arc du réseau
     *   lignes      : vecteur de paires (from, to) représentant chaque ligne
     *   nbVehicules : nombre de véhicules à affecter
     */
    static std::vector<std::vector<float>> buildCostMatrix(
        const std::vector<std::vector<struct Arc>>& mat,
        const std::vector<std::pair<int, int>>& lignes,
        int                                          nbVehicules);
}; 
