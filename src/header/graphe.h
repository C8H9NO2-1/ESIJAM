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

/**
 * @brief Fonction qui calcule les différentes matrices d'adjacences du graphe de la map
 * 
 * @param m Map dont on calcule les matrices d'adjacences
 * @param listeEntite Liste des entités de la map
 * @return Graphe Matrices d'adjacences
 */
Graphe matriceAdjacences(map m, ListeEntite *listeEntite);

/**
 * @brief Fonction qui renvoie le sommet non visité le plus proche
 * 
 * @param matrice Matrice d'adjacence
 * @param dejaVu Tableau indiquant si un sommet a déjà été visité
 * @param id Identifiant du sommet dont on cherche le plus proche voisin
 * @param m Map pour laquelle on a calculé la matrice d'adjacence
 * @return int Identifiant du plus proche voisin
 */
int plusProcheVoisin(int** matrice, bool* dejaVu, int id, map m);

/**
 * @brief Fonction qui met à jour les distances des sommets voisins
 * 
 * @param matrice Matrice d'adjacence
 * @param distance Tableau des distances
 * @param predecesseur Tableau des prédécesseurs
 * @param id Identifiant du sommet dont on met à jour les voisins
 * @param m Map pour laquelle on a calculé la matrice d'adjacence
 */
void mettreAJourDistance(int** matrice, int* distance, int *predecesseur, int id, map m);

/**
 * @brief Fonction qui calcule un chemin dans un graphe
 * 
 * @param depart Coordonnées de départ du graphe
 * @param arrivee Coordonnées d'arrivée du graphe
 * @param graphe Structure contenant les matrices d'adjacences nécessaires
 * @param m Map dont on a calculé la matrice d'adjacence
 * 
 * @return CheminEnnemi* Chemin calculé
 */
CheminEnnemi *calculeCheminEnnemi(Coordonnees depart, Coordonnees arrivee, Graphe graphe, map m);

/**
 * @brief Fonction qui calcule les chemins des ennemis pour une map donnée
 * 
 * @param graphe Structure contenant les matrices d'adjacences nécessaires
 * @param m Map dont on a calculé les matrices d'aadjacences
 * @return ListeCheminsEnnemis Chemins des ennemis
 */
ListeCheminsEnnemis *calculeCheminsEnnemis(Graphe graphe, map m);

/**
 * @brief Détruit la matrice d'adjacence
 * 
 * @param matrice Matrice d'adjacence à détruire
 * @param m Map dont on a calculé la matrice d'adjacence
 */
void freeMatriceAdjacence(int** matrice, map m);

/**
 * @brief Détruit le graphe
 * 
 * @param graphe Graphe à détruire
 * @param m Map dont on a calculé la matrice d'adjacence
 */
void freeGraphe(Graphe graphe, map m);

/**
 * @brief Détruit la liste des chemins des ennemis
 * 
 * @param listeCheminsEnnemis Liste des chemins des ennemis à détruire
 */
void freeListeCheminsEnnemis(ListeCheminsEnnemis *listeCheminsEnnemis);

#endif