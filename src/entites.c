#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"

//========== Coordonnées ==========

int minTableau(float *tab, int taille) {
    int i = 0;
    float min = tab[0];

    for (int j = 0; j < taille; j++) {
        if (tab[j] < min) {
            min = tab[j];
            i = j;
        }
    }

    return i;
}

//========== Entité ==========

ListeEntite* initialiserListeEntite(map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    ListeEntite *listeEntite = (ListeEntite *) malloc(sizeof(ListeEntite));

    listeEntite->entites = (Entite ****) malloc(sizeof(Entite ***) * largeur);

    for (int i = 0; i < largeur; i++) {
        listeEntite->entites[i] = (Entite ***) malloc(sizeof(Entite **) * hauteur);
    }
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            listeEntite->entites[i][j] = (Entite **) malloc(sizeof(Entite *) * 2);
        }
    }

    // for (int i = 0; i < largeur; i++) {
    //     for (int j = 0; j < hauteur; j++) {
    //         listeEntite->entites[i][j][0] = NULL;
    //         listeEntite->entites[i][j][1] = NULL;
    //     }
    // }

    // On déclare une entitée vide dans chaque case
    Entite *entiteVide = (Entite *) malloc(sizeof(Entite));
    entiteVide->pointsVie = -1;
    entiteVide->pointsAttaque = -1;
    entiteVide->allegeance = -1;
    entiteVide->typeEntite = -1;
    entiteVide->coordonnees = (Coordonnees) {-1, -1};

    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            listeEntite->entites[i][j][0] = entiteVide;
            listeEntite->entites[i][j][1] = entiteVide;
        }
    }

    return listeEntite;
}

void freeListeEntite(ListeEntite *listeEntite, map m) {
    int largeur = m.largeur / 2;
    int hauteur = m.hauteur / 2;

    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            free(listeEntite->entites[i][j]);
        }
    }
    for (int i = 0; i < largeur; i++) {
        free(listeEntite->entites[i]);
    }
    free(listeEntite->entites);
    free(listeEntite);
}

void initialiserEntite(Entite *entite, Allegeance allegeance, TypeEntite typeEntite, Coordonnees coordonnees, ListeEntite *listeEntite, texture_entite *texture) {
    // On initialise les variables qui ne dépendent pas du type d'entite avec lequel on travaille
    
    entite->coordonnees = coordonnees;
    entite->typeEntite = typeEntite;
    entite->texture = texture;
    entite->element = (ElementChemin *) malloc(sizeof(ElementChemin));
    // Si l'entite est une unite
    if (typeEntite == UNITE) {
        entite->pointsVie = POINTS_DE_VIE_INITIAUX;
        entite->pointsAttaque = POINTS_ATTAQUE;
        entite->allegeance = allegeance;
        listeEntite->entites[coordonnees.x][coordonnees.y][0] = entite;

    } else { // Si l'entite est un piege
        entite->pointsVie = DURABILITE_PIEGE;
        entite->pointsAttaque = DEGATS_INFLIGES;
        entite->allegeance = AMI;

        listeEntite->entites[coordonnees.x][coordonnees.y][1] = entite;
    }

    entite->indiceTexture = 0;
    entite->blink = false;
    entite->selectionne = false;
    entite->nouvelObjectif = false;
}

void detruireEntite(Entite *entite, ListeEntite *listeEntite) {
    if (entite->typeEntite == UNITE) {
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][0]->typeEntite = -1;
    } else {
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][1]->typeEntite = -1;
    }

    entite->pointsVie = -1;
    entite->pointsAttaque = -1;
    entite->allegeance = -1;
    entite->typeEntite = -1;
    entite->coordonnees.x = -1;
    entite->coordonnees.y = -1;
    
    // free(entite->texture);
    // free(entite->element);
}

void afficherListeEntite(ListeEntite *listeEntite, SDL_Renderer *renderer, map * M, camera *cam){
    for(int i = 0; i < M->largeur/2; i++){
        for(int j = 0; j < M->hauteur/2; j++){
            if(listeEntite->entites[i][j][0] != NULL)
                afficherEntite(listeEntite->entites[i][j][0], renderer, cam);
        }
    }
}

void afficherEntite(Entite *entite, SDL_Renderer *renderer, camera *cam) {
    SDL_Rect rect = {entite->coordonnees.x * 64 - (cam->x-(cam->w)/2)/cam->zoom, entite->coordonnees.y * 64 - ( cam->y-((cam->h)/2))/cam->zoom, TAILLE_TEXTURE_ENTITE-1, TAILLE_TEXTURE_ENTITE-1};
    // rect.x = rect.x * cam->zoom;
    // rect.y = rect.y * cam->zoom;
    switch (entite->typeEntite) {
    case UNITE:
    case PIEGE1:
    case PIEGE2:
        if (entite->blink) {
            SDL_RenderCopy(renderer, entite->texture->blink, NULL, &rect);
            entite->blink = false;
        } else if (entite->selectionne) {
            SDL_RenderCopy(renderer, entite->texture->selectionne[entite->indiceTexture], NULL, &rect);
        } else {
            SDL_RenderCopy(renderer, entite->texture->tab[entite->indiceTexture], NULL, &rect);
        }
        // SDL_RenderCopy(renderer, entite->texture->tab[entite->indiceTexture], NULL, &rect);
        break;
    // case PIEGE1:
    //     SDL_RenderCopy(renderer, entite->texture->tab[entite->indiceTexture], NULL, &rect);
    //     break;
    // case PIEGE2:
    //     SDL_RenderCopy(renderer, entite->texture->tab[entite->indiceTexture], NULL, &rect);
    //     break;
    default:
        break;
    }
}

void deplacementEntite(Entite *entite, Coordonnees coordonnees, map *m, ListeEntite *listeEntite) {
    if (entite->typeEntite == UNITE) {
        // On déclare une entitée vide dans chaque case
        Entite *entiteVide = (Entite *) malloc(sizeof(Entite));
        entiteVide->pointsVie = -1;
        entiteVide->pointsAttaque = -1;
        entiteVide->allegeance = -1;
        entiteVide->typeEntite = -1;
        entiteVide->coordonnees = (Coordonnees) {-1, -1};

        // On met à jour la liste des entités
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][0] = entiteVide;
        listeEntite->entites[coordonnees.x][coordonnees.y][0] = entite;

        // On met le sprite de l'unité à jour en fonction de sa direction
        if (entite->coordonnees.y < coordonnees.y) {
            entite->indiceTexture = 0;
        } else if (entite->coordonnees.y > coordonnees.y) {
            entite->indiceTexture = 1;
        } else if (entite->coordonnees.x > coordonnees.x) {
            entite->indiceTexture = 2;
        } else if (entite->coordonnees.x < coordonnees.x) {
            entite->indiceTexture = 3;
        }

        // On met à jour les coordonnées de l'entité
        entite->coordonnees = coordonnees;
    }
}

void attaquerEntite(Entite *entite, Entite *cible) {
    if (entite->allegeance != cible->allegeance) {
        cible->pointsVie -= entite->pointsAttaque;
        cible->blink = true;
    }
}

//TODO Il faut peut-être ajouter un champ dans la structure d'une entité indiquant quel chemin cette entité emprunte (pas si on fait un thread pour chaque unité)
void uniteEnnemie(Entite *entite, ListeEntite *listeEntite, map *m, bool *defeat, bool *exist) {
    int largeur = m->largeur / 2;
    int hauteur = m->hauteur / 2;

    // ElementChemin *element = entite->element;
    // ElementCheminEnnemi *element = chemin->premier;

    // bool attaque = false;

    //Si l'entité est détruite on sort de la fonction
    if (entite->pointsVie <= 0) {
        detruireEntite(entite, listeEntite);
        *exist = false;
        return;
    }

    // On récupère les coordonnées de l'entité
    int x = entite->coordonnees.x;
    int y = entite->coordonnees.y;

    // Il faut prendre en compte le cas où la case actuelle est un piège
    if (listeEntite->entites[x][y][1] != NULL) {
        if (listeEntite->entites[x][y][1]->typeEntite == PIEGE2) {
            attaquerEntite(listeEntite->entites[x][y][1], entite); // On se fait attaquer par le piège
        }
    }

    // Si le nexus est à proximité, on l'attaque
    if (entite->element->caseSuivante->coordonnees.x == largeur / 2 && entite->element->caseSuivante->coordonnees.y == hauteur / 2) {
        attaquerEntite(entite, listeEntite->entites[largeur / 2][hauteur / 2][0]);
        if (listeEntite->entites[largeur / 2][hauteur / 2][0]->pointsVie <= 0) {
            //TODO On détruit le nexus et le joueur à perdu
            detruireEntite(listeEntite->entites[largeur / 2][hauteur / 2][0], listeEntite);
            *defeat = true;
            return;
        }
        // continue;
        return;
    }

    // Si une unité alliée est à proximité de l'ennemi, on l'attaque
    // On en attaque une à la fois
    for (int i = -1; i <= 1; i++) {
        if (x + i >= 0 && x + i < largeur) {
            for (int j = -1; j <= 1; j++) {
                if (y + j >= 0 && y + j < hauteur) {
                    if (listeEntite->entites[x+i][y+j][0] != NULL) {
                        if (listeEntite->entites[x+i][y+j][0]->allegeance == AMI) {
                            attaquerEntite(entite, listeEntite->entites[x+i][y+j][0]);
                            return;
                        }
                    }
                }
            }
        }
    }
    
    // Sinon on essaie de se déplacer
    // On parcourt le chemin jusqu'à la fin
    if (entite->element->caseSuivante != NULL) {

        // attaque = false;

        // On récupère les coordonnées de la case suivante
        int xSuivant = entite->element->caseSuivante->coordonnees.x;
        int ySuivant = entite->element->caseSuivante->coordonnees.y;

        // On doit gérer le cas où la prochaine case est un mur cassable
        if (listeEntite->entites[xSuivant][ySuivant][1] != NULL) {
            if (listeEntite->entites[xSuivant][ySuivant][1]->typeEntite == PIEGE1) {
                attaquerEntite(entite, listeEntite->entites[xSuivant][ySuivant][1]); // On détruit le mur cassable
                // Si le mur n'a plus de points de vie, on le détruit
                if (listeEntite->entites[xSuivant][ySuivant][1]->pointsVie <= 0) {
                    detruireEntite(listeEntite->entites[xSuivant][ySuivant][1], listeEntite);
                }
                // continue;
                return;
            }
        }

        // On déplace l'unité ennemie si il n'y a pas d'autres unités sur cette case
        if (listeEntite->entites[xSuivant][ySuivant][0]->typeEntite == -1) {
            deplacementEntite(entite, (Coordonnees) {xSuivant, ySuivant}, m, listeEntite);
            entite->element = entite->element->caseSuivante;
            // element = element->caseSuivante;
        }
    }
}

void nouveauCheminAmi(Entite *entite, ListeEntite *listeEntite, map *m, Graphe graphe, Coordonnees destination) {
    CheminAmi *chemin = calculeCheminAmi(entite->coordonnees, destination, graphe, *m);
    entite->element = chemin->premier;
    if (!chemin->valide) {
        entite->element->caseSuivante = NULL;
        printf("Chemin invalide\n");
    }
}

void uniteAmie(Entite *entite, ListeEntite *listeEntite, map *m, bool *exist) {
    int largeur = m->largeur / 2;
    int hauteur = m->hauteur / 2;

    // ElementChemin *element = entite->element;

    // printf("Test\n");

    // On récupère les coordonnées de l'entité
    int x = entite->coordonnees.x;
    int y = entite->coordonnees.y;

    //Si l'entité est détruite on sort de la fonction
    if (entite->pointsVie <= 0) {
        detruireEntite(entite, listeEntite);
        *exist = false;
        return;
    }

    // Si une unité ennemie est à proximité de l'allié, on l'attaque
    // On en attaque une à la fois
    for (int i = -1; i <= 1; i++) {
        if (x + i >= 0 && x + i < largeur) {
            for (int j = -1; j <= 1; j++) {
                if (y + j >= 0 && y + j < hauteur) {
                    if (listeEntite->entites[x+i][y+j][0] != NULL) {
                        if (listeEntite->entites[x+i][y+j][0]->allegeance == ENNEMI) {
                            attaquerEntite(entite, listeEntite->entites[x+i][y+j][0]);
                            return;
                        }
                    }
                }
            }
        }
    }
    
    // On parcourt le chemin jusqu'à la fin
    if (entite->element->caseSuivante != NULL && entite->element->caseSuivante->caseSuivante != NULL) {

        // On récupère les coordonnées de la case suivante
        int xSuivant = entite->element->caseSuivante->coordonnees.x;
        int ySuivant = entite->element->caseSuivante->coordonnees.y;

        // On déplace l'unité ennemie si il n'y a pas d'autres unités sur cette case
        if (listeEntite->entites[xSuivant][ySuivant][0]->typeEntite == -1) {
            deplacementEntite(entite, (Coordonnees) {xSuivant, ySuivant}, m, listeEntite);
            entite->element = entite->element->caseSuivante;
            // element = element->caseSuivante;
        }
    }
}

void selectionneEntite(ListeEntite *listeEntite, Coordonnees coordonnees, map *M, camera *cam) {
    int largeur = M->largeur / 2;
    int hauteur = M->hauteur / 2;

    // On récupère les coordonnées de la souris
    int x = coordonnees.x / cam->zoom;
    int y = coordonnees.y / cam->zoom;

    // On parcours les entités
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            // Si l'entité est une unité
            if (listeEntite->entites[i][j][0] != NULL) {
                if (listeEntite->entites[i][j][0]->typeEntite == UNITE && listeEntite->entites[i][j][0]->allegeance == AMI) {
                    // Si c'est le nexus on ne le sélectionne pas
                    if (i == largeur / 2 && j == hauteur / 2) {
                        continue;
                    }
                    // On convertit les coordonnées de l'entité en coordonnées de la carte
                    int xEntite = listeEntite->entites[i][j][0]->coordonnees.x * 64 - (cam->x-(cam->w)/2)/cam->zoom;
                    int yEntite = listeEntite->entites[i][j][0]->coordonnees.y * 64 - ( cam->y-((cam->h)/2))/cam->zoom;

                    // On vérifie si la souris est sur l'entité
                    if (x >= xEntite - 64 && x <= xEntite + 64 && y >= yEntite - 64 && y <= yEntite + 64) {
                        // On sélectionne l'entité
                        listeEntite->entites[i][j][0]->selectionne = true;
                        return;
                    }
                }
            }
        }
    }
}

void donnerObjectif(ListeEntite *listeEntite, Coordonnees coordonnees, map *M, camera *cam) {
    int largeur = M->largeur / 2;
    int hauteur = M->hauteur / 2;

    // On récupère les coordonnées de la souris
    int x = coordonnees.x / cam->zoom;
    int y = coordonnees.y / cam->zoom;

    // On parcours les entités
    for (int i = 0; i < largeur; i++) {
        for (int j = 0; j < hauteur; j++) {
            // Si l'entité est une unité
            if (listeEntite->entites[i][j][0] != NULL) {
                if (listeEntite->entites[i][j][0]->typeEntite == UNITE && listeEntite->entites[i][j][0]->allegeance == AMI && listeEntite->entites[i][j][0]->selectionne) {
                    // On donne à l'unité un nouvel objectif et on la déselectionne
                    listeEntite->entites[i][j][0]->selectionne = false;
                    listeEntite->entites[i][j][0]->nouvelObjectif = true;
                    listeEntite->entites[i][j][0]->objectif = (Coordonnees) {(x + (cam->x-(cam->w)/2)/cam->zoom) / 64, (y + (cam->y-(cam->h)/2)/cam->zoom) / 64};
                    listeEntite->entites[i][j][0]->element->caseSuivante = NULL;
                }
            }
        }
    }
}

//========== Fin Entité ==========

// //========== Unité ==========

// void initialiserUnite(Unite *unite, Allegeance allegeance, Coordonnees coordonnees) {
//     unite->pointsVie = POINTS_DE_VIE_INITIAUX;
//     unite->pointsAttaque = POINTS_ATTAQUE;
//     unite->allegeance = allegeance;
//     unite->coordonnees = coordonnees;
// }

// void detruireUnite(Unite *unite) {
//     unite->pointsVie = -1;
//     unite->pointsAttaque = -1;
//     unite->allegeance = -1;
//     unite->coordonnees.x = -1;
//     unite->coordonnees.y = -1;
// }

// void afficherUnite(Unite *unite, SDL_Renderer *renderer, texture_unite *tU) {
//     SDL_Rect rect = {unite->coordonnees.x, unite->coordonnees.y, TAILLE_TEXTURE_UNITE-1, TAILLE_TEXTURE_UNITE-1};
//     if (unite->allegeance == AMI) {
//         SDL_RenderCopy(renderer, tU->textureAmi[0], NULL, &rect);
//     } else {
//         SDL_RenderCopy(renderer, tU->textureEnnemi[0], NULL, &rect);
//     }
// }

// void attaquerUnite(Unite *unite, Unite *cible) {
//     if (unite->allegeance != cible->allegeance) {
//         cible->pointsVie -= unite->pointsAttaque;
//     }
// }

// //========== Piège ==========

// void initialiserPiege(Piege *piege, TypePiege typePiege, Coordonnees coordonnees) {
//     piege->durabilite = DURABILITE_PIEGE;
//     piege->degatsInfliges = DEGATS_INFLIGES;
//     piege->typePiege = typePiege;
//     piege->coordonnees = coordonnees;
// }

// void detruirePiege(Piege *piege) {
//     piege->durabilite = -1;
//     piege->degatsInfliges = -1;
//     piege->coordonnees.x = -1;
//     piege->coordonnees.y = -1;
// }

// void afficherPiege(Piege *piege, SDL_Renderer *renderer, texture_piege *tP) {
//     SDL_Rect rect = {0, 0, TAILLE_TEXTURE_PIEGE-1, TAILLE_TEXTURE_PIEGE-1};
//     SDL_Rect dstRect = {piege->coordonnees.x, piege->coordonnees.y, -1, -1};
//     if (piege->typePiege == PIEGE1) {
//         SDL_RenderCopy(renderer, tP->texturePiege1[0], &rect, &dstRect);
//     } else {
//         SDL_RenderCopy(renderer, tP->texturePiege2[1], &rect, &dstRect);
//     }
// }

// void activerPiege(Piege *piege, Unite *unite) {
//     if (unite->allegeance == ENNEMI) {
//         unite->pointsVie -= piege->degatsInfliges;
//         piege->durabilite--;
//     }
// }
