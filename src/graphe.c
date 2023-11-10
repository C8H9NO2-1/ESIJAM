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
    // On calcule la distance euclidienne entre la case et la case objectif
    return sqrt(pow(tuile.x - arrivee.x, 2) + pow(tuile.y - arrivee.y, 2));
}

int** matriceAdjacence(map m, ListeEntite *listeEntite) {
    int nbreLigne = m.hauteur * m.largeur;
    
    // On attribue de la mémoire pour la matrice
    int** matrice = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matrice[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    // On met la diagonale à 0
    for (int i = 0; i < nbreLigne; i++) {
        matrice[i][i] = 0;
    }

    // On met toutes les cases qui sont des murs ou des murs posables à -1
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) != 0 || listeEntite->entites[i][j][1]->typeEntite == PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matrice[idVoisins[k]][id] = -1;
                    }
                }
            }
        }
    }

    // On met les cases adjacentes à 1 + une heuristique
    //! Code à vérifier!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for (int i = 0; i < m.largeur; i++) {
        for (int j = 0; j < m.hauteur; j++) {
            if ((m.tab[i + j * m.largeur] % 4) == 0 && listeEntite->entites[i][j][1]->typeEntite != PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[8] = {id - m.largeur - 1, id - m.largeur, id - m.largeur + 1, id - 1, id + 1, id + m.largeur - 1, id + m.largeur, id + m.largeur + 1};

                for (int k = 0; k < 8; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne && matrice[id][idVoisins[k]] != -1) {
                        int heur = heuristique((Coordonnees) {i, j}, (Coordonnees) {12, 12});
                        matrice[idVoisins[k]][id] = 1 + heur;
                    }
                }
            }
        }
    }

    return matrice;
}

void freeMatriceAdjacence(int** matrice, map m) {
    int nbreLigne = m.hauteur * m.largeur;

    for (int i = 0; i < nbreLigne; i++) {
        free(matrice[i]);
    }

    free(matrice);
}