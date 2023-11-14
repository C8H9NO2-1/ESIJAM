#ifndef __GRAPHE_H__
#define __GRAPHE_H__

/**
 * @brief Fonction qui attribue un identifiant unique à chaque case de la map
 * 
 * @param tuile Case à identifier
 * @param m Map
 * @return int Identifiant de la case

 */
int identifiantCase(Coordonnees tuile, map m);

/**
 * @brief Fonction de calcul d'heuristique
 * 
 * @param tuile Case à laquelle on attirue l'heuristique
 * @param arrivee Case objectif
 * @return int Heuristique de la case
 */
int heuristique(Coordonnees tuile, Coordonnees arrivee);

struct Graphe {
    int** matriceAdjacenceEnemi1;
    int** matriceAdjacenceEnemi2; // Cette matrice est la même que celle de l'enemi 1 sauf que l'on ne tient pas compte des murs cassables
    int** matriceAdjacenceAllie;
};
typedef struct Graphe Graphe;

/**
 * @brief Fonction qui calcule les différentes matrices d'adjacences du graphe de la map
 * 
 * @param m Map dont on calcule les matrices d'adjacences
 * @param listeEntite Liste des entités de la map
 * @return Graphe Matrices d'adjacences
 */
Graphe matriceAdjacences(map m, ListeEntite *listeEntite);

/**
 * @brief Détruit la matrice d'adjacence
 * 
 * @param matrice Matrice d'adjacence à détruire
 * @param m Map dont on a calculé la matrice d'adjacence
 */
void freeMatriceAdjacence(int** matrice, map m);

#endif