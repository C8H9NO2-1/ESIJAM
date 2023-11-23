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

void initialiserEntite(Entite *entite, Allegeance allegeance, TypeEntite typeEntite, Coordonnees coordonnees, ListeEntite *listeEntite) {
    // On initialise les variables qui ne dépendent pas du type d'entite avec lequel on travaille
    entite->coordonnees = coordonnees;
    entite->typeEntite = typeEntite;
    entite->texture = (texture_entite *) malloc(sizeof(texture_entite));
    entite->element = (ElementCheminEnnemi *) malloc(sizeof(ElementCheminEnnemi));

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
}

void detruireEntite(Entite *entite, ListeEntite *listeEntite) {
    if (entite->typeEntite == UNITE) {
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][0] = NULL;
    } else {
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][1] = NULL;
    }

    entite->pointsVie = -1;
    entite->pointsAttaque = -1;
    entite->allegeance = -1;
    entite->typeEntite = -1;
    entite->coordonnees.x = -1;
    entite->coordonnees.y = -1;
    
    free(entite->texture);
    free(entite->element);
}

void afficherEntite(Entite *entite, SDL_Renderer *renderer) {
    SDL_Rect rect = {entite->coordonnees.x * 64, entite->coordonnees.y * 64, TAILLE_TEXTURE_ENTITE-1, TAILLE_TEXTURE_ENTITE-1};

    switch (entite->typeEntite) {
    case UNITE:
        if (entite->allegeance == AMI) {
            SDL_RenderCopy(renderer, entite->texture->textureAmi[0], NULL, &rect);
        } else {
            SDL_RenderCopy(renderer, entite->texture->textureEnnemi[0], NULL, &rect);
        }
        break;
    case PIEGE1:
        SDL_RenderCopy(renderer, entite->texture->texturePiege1[0], NULL, &rect);
        break;
    case PIEGE2:
        SDL_RenderCopy(renderer, entite->texture->texturePiege2[0], NULL, &rect);
        break;
    default:
        break;
    }
}

void deplacementEntite(Entite *entite, Coordonnees coordonnees, map *m, ListeEntite *listeEntite) {
    if (entite->typeEntite == UNITE) {
        // On met à jour la liste des entités
        listeEntite->entites[entite->coordonnees.x][entite->coordonnees.y][0] = NULL;
        listeEntite->entites[coordonnees.x][coordonnees.y][0] = entite;

        // On met à jour les coordonnées de l'entité
        entite->coordonnees = coordonnees;
    }
}

void attaquerEntite(Entite *entite, Entite *cible) {
    if (entite->allegeance != cible->allegeance) {
        cible->pointsVie -= entite->pointsAttaque;
    }
}

//TODO Il faut peut-être ajouter un champ dans la structure d'une entité indiquant quel chemin cette entité emprunte (pas si on fait un thread pour chaque unité)
void uniteEnnemie(Entite *entite, ListeEntite *listeEntite, map *m, CheminEnnemi *chemin, bool *defeat) {
    int largeur = m->largeur / 2;
    int hauteur = m->hauteur / 2;

    ElementCheminEnnemi *element = entite->element;
    // ElementCheminEnnemi *element = chemin->premier;

    bool attaque = false;

    // On parcourt le chemin jusqu'à la fin
    if (element != NULL) { //! Je ne sais pas si on doit faire ça ou un simple if (dans le deuxième cas if faut refaire un peu le code) 
                                            //* Je penche plutôt pour un if

        attaque = false;

        //Si l'entité est détruite on sort de la fonction
        if (entite->pointsVie <= 0) {
            detruireEntite(entite, listeEntite);
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
        if (element->caseSuivante->coordonnees.x == largeur / 2 && element->caseSuivante->coordonnees.y == hauteur / 2) {
            attaquerEntite(entite, listeEntite->entites[largeur / 2][hauteur / 2][0]);
            if (listeEntite->entites[largeur / 2][hauteur / 2][0]->pointsVie <= 0) {
                //TODO On détruit le nexus et le joueur à perdu
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
                                attaque = true;
                            }
                        }
                    }
                }
            }
        }

        if (attaque) {
            // continue;
            return;
        }

        // On récupère les coordonnées de la case suivante
        int xSuivant = element->caseSuivante->coordonnees.x;
        int ySuivant = element->caseSuivante->coordonnees.y;

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

        // On déplace l'unité ennemie
        deplacementEntite(entite, (Coordonnees) {xSuivant, ySuivant}, m, listeEntite);

        entite->element = entite->element->caseSuivante;
        // element = element->caseSuivante;
        
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
