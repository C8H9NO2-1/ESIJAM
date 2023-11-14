#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/graphe.h"

int identifiantCase(Coordonnees tuile, map m) {
    return tuile.x + (tuile.y * m.largeur);
}

int heuristique(Coordonnees tuile, Coordonnees arrivee) {
    // On calcule la distance de Manhattan entre la case et l'arrivée
    return abs(tuile.x - arrivee.x) + abs(tuile.y - arrivee.y);
}

Graphe matriceAdjacences(map m, ListeEntite *listeEntite) {
    int nbreLigne = m.hauteur * m.largeur;
    
    //========== Ennemi 1 ==========
    // On attribue de la mémoire pour la première matrice des ennemis
    int** matriceEnnemi1 = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceEnnemi1[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    // On met la diagonale à 0
    for (int i = 0; i < nbreLigne; i++) {
        matriceEnnemi1[i][i] = 0;
    }

    // On met toutes les cases qui sont des murs ou des murs posables à -1
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) != 0 || listeEntite->entites[i][j][1]->typeEntite == PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matriceEnnemi1[idVoisins[k]][id] = -1;
                    }
                }
            }
        }
    }

    // On met les cases adjacentes à 1 + une heuristique
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) == 0 && listeEntite->entites[i][j][1]->typeEntite != PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        int heur = heuristique((Coordonnees) {i, j}, (Coordonnees) {m.largeur / 2, m.hauteur / 2});
                        matriceEnnemi1[idVoisins[k]][id] = 1 + heur;
                    }
                }
            }
        }
    }

    //========== Ennemi 2 ==========
    // On attribue de la mémoire pour la deuxième matrice des ennemis
    int** matriceEnnemi2 = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceEnnemi2[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    // On met la diagonale à 0
    for (int i = 0; i < nbreLigne; i++) {
        matriceEnnemi2[i][i] = 0;
    }

    // On met toutes les cases qui sont des murs ou des murs posables à -1
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) != 0) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matriceEnnemi2[idVoisins[k]][id] = -1;
                    }
                }
            }
        }
    }

    // On met les cases adjacentes à 1 + une heuristique
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) == 0) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        int heur = heuristique((Coordonnees) {i, j}, (Coordonnees) {m.largeur / 2, m.hauteur / 2});
                        matriceEnnemi2[idVoisins[k]][id] = 1 + heur;
                    }
                }
            }
        }
    }

    //========== Allié ==========
    // On attribue de la mémoire pour la première matrice des ennemis
    int** matriceAllie = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceAllie[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    // On met la diagonale à 0
    for (int i = 0; i < nbreLigne; i++) {
        matriceAllie[i][i] = 0;
    }

    // On met toutes les cases qui sont des murs ou des murs posables à -1
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) != 0 || listeEntite->entites[i][j][1]->typeEntite == PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matriceAllie[idVoisins[k]][id] = -1;
                    }
                }
            }
        }
    }

    // On met les cases adjacentes à 1 + une heuristique
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) == 0 && listeEntite->entites[i][j][1]->typeEntite != PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matriceAllie[idVoisins[k]][id] = 1;
                    }
                }
            }
        }
    }

    return (Graphe) {matriceEnnemi1, matriceEnnemi2, matriceAllie};
}

void freeMatriceAdjacence(int** matrice, map m) {
    int nbreLigne = m.hauteur * m.largeur;

    for (int i = 0; i < nbreLigne; i++) {
        free(matrice[i]);
    }

    free(matrice);
}