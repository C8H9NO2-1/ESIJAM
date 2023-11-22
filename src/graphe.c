#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/graphe.h"
#include "header/pile.h"

int identifiantCase(Coordonnees tuile, map m) {
    return tuile.x + (tuile.y * m.largeur / 2);
}

int heuristique(Coordonnees tuile, Coordonnees arrivee) {
    // On calcule la distance de Manhattan entre la case et l'arrivée
    return abs(tuile.x - arrivee.x) + abs(tuile.y - arrivee.y);
    // return sqrt((tuile.x - arrivee.x) * (tuile.x - arrivee.x) + (tuile.y - arrivee.y) * (tuile.y - arrivee.y));
}

Graphe matriceAdjacences(map m, ListeEntite *listeEntite) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;
    
    int nbreLigne = hauteur * largeur;
    
    //========== Ennemi 1 ==========
    // On attribue de la mémoire pour la première matrice des ennemis
    int** matriceEnnemi1 = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceEnnemi1[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    //========== Ennemi 2 ==========
    // On attribue de la mémoire pour la deuxième matrice des ennemis
    int** matriceEnnemi2 = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceEnnemi2[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    //========== Allié ==========
    // On attribue de la mémoire pour la première matrice des ennemis
    int** matriceAllie = (int **) malloc(sizeof(int *) * nbreLigne);
    for(int i = 0; i < nbreLigne; i++) {
        matriceAllie[i] = (int *) malloc(nbreLigne * sizeof(int));
    }

    // On met toutes les cases à -INFINITY
    for (int i = 0; i < nbreLigne; i++) {
        for (int j = 0; j < nbreLigne; j++) {
            matriceEnnemi1[i][j] = -1;
            matriceEnnemi2[i][j] = -1;
            matriceAllie[i][j] = -1;
        }
    }

    // On met toutes les diagonales à 0
    for (int i = 0; i < nbreLigne; i++) {
        matriceEnnemi1[i][i] = 0;
        matriceEnnemi2[i][i] = 0;
        matriceAllie[i][i] = 0;
    }

    //========== Ennemi 1 ==========
    // On met les cases adjacentes à 1 + une heuristique
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            if ((m.tab[2 * i + 2 * j * m.largeur] % 4) == 0 && listeEntite->entites[i][j][1]->typeEntite != PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[4] = {id - largeur, id - 1, id + 1, id + largeur};

                for (int k = 0; k < 4; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        int heur = heuristique((Coordonnees) {i, j}, (Coordonnees) {largeur / 2, hauteur / 2});
                        // heur = 0;
                        matriceEnnemi1[idVoisins[k]][id] = 1 + heur;
                    }
                }
            }
        }
    }

    //========== Ennemi 2 ==========
    // On met les cases adjacentes à 1 + une heuristique
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            if ((m.tab[2 * i + 2 * j * m.largeur] % 4) == 0) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[4] = {id - largeur, id - 1, id + 1, id + largeur};

                for (int k = 0; k < 4; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        int heur = heuristique((Coordonnees) {i, j}, (Coordonnees) {largeur / 2, hauteur / 2});
                        // heur = 0;
                        matriceEnnemi2[idVoisins[k]][id] = 1 + heur;
                    }
                }
            }
        }
    }

    //========== Allié ==========
    // On met les cases adjacentes à 1
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            if ((m.tab[2 * i + 2 * j * m.largeur] % 4) == 0 && listeEntite->entites[i][j][1]->typeEntite != PIEGE1) {
                int id = identifiantCase((Coordonnees) {i, j}, m);
                int idVoisins[4] = {id - largeur, id - 1, id + 1, id + largeur};

                for (int k = 0; k < 4; k++) {
                    if (idVoisins[k] >= 0 && idVoisins[k] < nbreLigne) {
                        matriceAllie[idVoisins[k]][id] = 1;
                    }
                }
            }
        }
    }

    // SDL_Delay(100);
    // printf("Calcul des matrices d'adjacences fait\n");
    // SDL_Delay(500);

    return (Graphe) {nbreLigne, matriceEnnemi1, matriceEnnemi2, matriceAllie};
}

int plusProcheVoisin(int** matrice, bool* dejaVu, int id, map m) {
    int min = INT_MAX;
    int idMin = -1;

    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    int idVoisins[4] = {id - largeur, id - 1, id + 1, id + largeur};
    for (int i = 0; i < 4; i++) {
        if (matrice[id][idVoisins[i]] != -1) {
            if (idVoisins[i] >= 0 && idVoisins[i] < largeur * hauteur && !dejaVu[idVoisins[i]] && matrice[id][idVoisins[i]] < min) {
                min = matrice[id][idVoisins[i]];
                idMin = idVoisins[i];
            }
        }
    }

    return idMin;
}

void mettreAJourDistance(int** matrice, int* distance, int *predecesseur, int id, map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    int idVoisins[4] = {id - largeur, id - 1, id + 1, id + largeur};
    for (int i = 0; i < 4; i++) {
        if (idVoisins[i] >= 0 && idVoisins[i] < largeur * hauteur && distance[idVoisins[i]] > distance[id] + matrice[id][idVoisins[i]]) {
            distance[idVoisins[i]] = distance[id] + matrice[id][idVoisins[i]];
            predecesseur[idVoisins[i]] = id;
        }
    }

}

CheminEnnemi *calculeCheminEnnemi(Coordonnees depart, Coordonnees arrivee, Graphe graphe, map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;
    
    CheminEnnemi *chemin = malloc(sizeof(*chemin));
    ElementCheminEnnemi *element = malloc(sizeof(*element));
    element->coordonnees = depart;
    element->caseSuivante = NULL;
    chemin->premier = element;

    // On essaie d'abord de trouver un chemin avec la matrice Ennemi 1

    //========== Initialisation ==========
    // On initialise le tableau des distances à l'infini sauf pour le départ
    int *distance = malloc(sizeof(int) * graphe.nbreLigne);
    for (int i = 0; i < graphe.nbreLigne; i++) {
        distance[i] = INT_MAX;
    }
    int idDepart = identifiantCase(depart, m);
    distance[idDepart] = 0;

    // On initialise le tableau pour savoir si un sommet a déjà été visité
    bool *dejaVu = malloc(sizeof(bool) * graphe.nbreLigne);
    for (int i = 0; i < graphe.nbreLigne; i++) {
        dejaVu[i] = false;
    }

    // On initialise le tableau des précédents à -1
    int *precedent = malloc(sizeof(int) * graphe.nbreLigne);
    for (int i = 0; i < graphe.nbreLigne; i++) {
        precedent[i] = -1;
    }

    // On fait les dernières initialisations
    int idArrivee = identifiantCase(arrivee, m);
    int idSommet = idDepart;
    Pile *pile = initialiserPile();
    empiler(pile, idSommet);

    // SDL_Delay(100);
    // printf("Première initialisation terminée\n");

    //========== Algorithme de Dijkstra ==========
    while (idSommet != idArrivee && !estPileVide(pile)) {
        mettreAJourDistance(graphe.matriceAdjacenceEnemi1, distance, precedent, idSommet, m);

        if (!dejaVu[idSommet]) {
            empiler(pile, idSommet);
        }
        dejaVu[idSommet] = true;

        idSommet = plusProcheVoisin(graphe.matriceAdjacenceEnemi1, dejaVu, idSommet, m);
        if (idSommet == -1) {
            idSommet = depiler(pile);
        }

        // printf("x = %d / y = %d\n", idSommet % largeur, idSommet / largeur);
    }

    // SDL_Delay(100);
    // printf("Graphe parcouru et fini\n");

    if (idSommet == -1) {
        return chemin;
    }

    if (idSommet == idArrivee) {
        // On a trouvé un chemin
        while (idSommet != idDepart) {
            element = malloc(sizeof(*element));
            element->coordonnees = (Coordonnees) {idSommet % largeur, idSommet / largeur};
            element->caseSuivante = chemin->premier;
            chemin->premier = element;
            idSommet = precedent[idSommet];

            // printf("%d\n", idSommet);

        }

        element = malloc(sizeof(*element));
        element->coordonnees = (Coordonnees) {idSommet % largeur, idSommet / largeur};
        element->caseSuivante = chemin->premier;
        chemin->premier = element;

        // printf("On a réussi à faire un chemin\n");
    } else {
        // On essaie de trouver un chemin avec la matrice Ennemi 2

        //========== Initialisation ==========
        // On initialise le tableau des distances à l'infini sauf pour le départ
        for (int i = 0; i < graphe.nbreLigne; i++) {
            distance[i] = INT_MAX;
        }
        idDepart = identifiantCase(depart, m);
        distance[idDepart] = 0;

        // On initialise le tableau pour savoir si un sommet a déjà été visité
        for (int i = 0; i < graphe.nbreLigne; i++) {
            dejaVu[i] = false;
        }

        // On initialise le tableau des précédents à -1
        for (int i = 0; i < graphe.nbreLigne; i++) {
            precedent[i] = -1;
        }

        // On fait les dernières initialisations
        idArrivee = identifiantCase(arrivee, m);
        idSommet = idDepart;

        //========== Algorithme de Dijkstra ==========
        while (idSommet != idArrivee && idSommet != -1) {
            mettreAJourDistance(graphe.matriceAdjacenceEnemi1, distance, precedent, idSommet, m);

            if (!dejaVu[idSommet]) {
                empiler(pile, idSommet);
            }
            dejaVu[idSommet] = true;

            idSommet = plusProcheVoisin(graphe.matriceAdjacenceEnemi1, dejaVu, idSommet, m);
            if (idSommet == -1) {
                idSommet = depiler(pile);
            }

            // printf("x = %d / y = %d\n", idSommet % largeur, idSommet / largeur);
        }

        // On a forcément trouvé un chemin
        while (idSommet != idDepart) {
            element = malloc(sizeof(*element));
            element->coordonnees = (Coordonnees) {idSommet % largeur, idSommet / largeur};
            element->caseSuivante = chemin->premier;
            chemin->premier = element;
            idSommet = precedent[idSommet];
        }

        element = malloc(sizeof(*element));
        element->coordonnees = (Coordonnees) {idSommet % largeur, idSommet / largeur};
        element->caseSuivante = chemin->premier;
        chemin->premier = element;
    }

    free(distance);
    free(dejaVu);
    free(precedent);
    free(pile);

    return chemin;
}

ListeCheminsEnnemis* calculeCheminsEnnemis(Graphe graphe, map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    ListeCheminsEnnemis *listeCheminsEnnemis = malloc(sizeof(*listeCheminsEnnemis));

    Coordonnees arrivee = {largeur / 2, hauteur / 2}; // Coordonnées du Nexus

    // Chemin 1 (Celui qui part du haut)
    // listeCheminsEnnemis->chemin1 = calculeCheminEnnemi((Coordonnees) {largeur / 2, 0}, (Coordonnees) {6, 5}, graphe, m);
    listeCheminsEnnemis->chemin1 = calculeCheminEnnemi((Coordonnees) {largeur / 2, 0}, arrivee, graphe, m);

    // Chemin 2 (Celui qui part de la droite)
    // printf("Départ: x = %d / y = %d\n", largeur - 1, hauteur / 2);
    // SDL_Delay(500);
    listeCheminsEnnemis->chemin2 = calculeCheminEnnemi((Coordonnees) {largeur - 1, hauteur / 2}, arrivee, graphe, m);

    // Chemin 3 (Celui qui part du bas)
    listeCheminsEnnemis->chemin3 = calculeCheminEnnemi((Coordonnees) {largeur / 2, hauteur - 1}, arrivee, graphe, m);

    // Chemin 4 (Celui qui part de la gauche)
    listeCheminsEnnemis->chemin4 = calculeCheminEnnemi((Coordonnees) {0, hauteur / 2}, arrivee, graphe, m);
    // listeCheminsEnnemis->chemin4 = calculeCheminEnnemi((Coordonnees) {0, hauteur / 2}, (Coordonnees) {3, 13}, graphe, m);
    // listeCheminsEnnemis->chemin4 = calculeCheminEnnemi((Coordonnees) {3, hauteur / 2}, (Coordonnees) {3, 13}, graphe, m);

    return listeCheminsEnnemis;
}

void freeMatriceAdjacence(int** matrice, map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    int nbreLigne = hauteur * largeur;

    for (int i = 0; i < nbreLigne; i++) {
        free(matrice[i]);
    }

    free(matrice);
}

void freeGraphe(Graphe graphe, map m) {
    freeMatriceAdjacence(graphe.matriceAdjacenceAllie, m);
    freeMatriceAdjacence(graphe.matriceAdjacenceEnemi1, m);
    freeMatriceAdjacence(graphe.matriceAdjacenceEnemi2, m);
}

void freeListeCheminsEnnemis(ListeCheminsEnnemis *listeCheminsEnnemis) {
    while (listeCheminsEnnemis->chemin1->premier != NULL) {
        ElementCheminEnnemi *element = listeCheminsEnnemis->chemin1->premier;
        listeCheminsEnnemis->chemin1->premier = listeCheminsEnnemis->chemin1->premier->caseSuivante;
        free(element);
    }
    free(listeCheminsEnnemis->chemin1);

    while (listeCheminsEnnemis->chemin2->premier != NULL) {
        ElementCheminEnnemi *element = listeCheminsEnnemis->chemin2->premier;
        listeCheminsEnnemis->chemin2->premier = listeCheminsEnnemis->chemin2->premier->caseSuivante;
        free(element);
    }
    free(listeCheminsEnnemis->chemin2);

    while (listeCheminsEnnemis->chemin3->premier != NULL) {
        ElementCheminEnnemi *element = listeCheminsEnnemis->chemin3->premier;
        listeCheminsEnnemis->chemin3->premier = listeCheminsEnnemis->chemin3->premier->caseSuivante;
        free(element);
    }
    free(listeCheminsEnnemis->chemin3);

    while (listeCheminsEnnemis->chemin4->premier != NULL) {
        ElementCheminEnnemi *element = listeCheminsEnnemis->chemin4->premier;
        listeCheminsEnnemis->chemin4->premier = listeCheminsEnnemis->chemin4->premier->caseSuivante;
        free(element);
    }
    free(listeCheminsEnnemis->chemin4);

    free(listeCheminsEnnemis);
}