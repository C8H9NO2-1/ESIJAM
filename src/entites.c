#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/texture_entites.h"

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

ListeEntite* initialiserListeEntite() {
    ListeEntite *listeEntite = (ListeEntite *) malloc(sizeof(ListeEntite));

    listeEntite->entites = (Entite ****) malloc(sizeof(Entite ***) * WIDTH_MAP);

    for (int i = 0; i < WIDTH_MAP; i++) {
        listeEntite->entites[i] = (Entite ***) malloc(sizeof(Entite **) * HEIGHT_MAP);
    }
    for (int i = 0; i < WIDTH_MAP; i++) {
        for (int j = 0; j < HEIGHT_MAP; j++) {
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

    for (int i = 0; i < WIDTH_MAP; i++) {
        for (int j = 0; j < HEIGHT_MAP; j++) {
            listeEntite->entites[i][j][0] = entiteVide;
            listeEntite->entites[i][j][1] = entiteVide;
        }
    }

    return listeEntite;
}

void freeListeEntite(ListeEntite *listeEntite) {
    for (int i = 0; i < WIDTH_MAP; i++) {
        for (int j = 0; j < HEIGHT_MAP; j++) {
            free(listeEntite->entites[i][j]);
        }
    }
    for (int i = 0; i < WIDTH_MAP; i++) {
        free(listeEntite->entites[i]);
    }
    free(listeEntite->entites);
    free(listeEntite);
}

void initialiserEntite(Entite *entite, Allegeance allegeance, TypeEntite typeEntite, Coordonnees coordonnees, ListeEntite *listeEntite) {
    // On initialise les variables qui ne dépendent pas du type d'entite avec lequel on travaille
    entite->coordonnees = coordonnees;
    entite->typeEntite = typeEntite;

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
}

void afficherEntite(Entite *entite, SDL_Renderer *renderer, texture_entite *tE) {
    SDL_Rect rect = {entite->coordonnees.x, entite->coordonnees.y, TAILLE_TEXTURE_ENTITE-1, TAILLE_TEXTURE_ENTITE-1};

    switch (entite->typeEntite) {
    case UNITE:
        if (entite->allegeance == AMI) {
            SDL_RenderCopy(renderer, tE->textureAmi[0], NULL, &rect);
        } else {
            SDL_RenderCopy(renderer, tE->textureEnnemi[0], NULL, &rect);
        }
        break;
    case PIEGE1:
        SDL_RenderCopy(renderer, tE->texturePiege1[0], NULL, &rect);
        break;
    case PIEGE2:
        SDL_RenderCopy(renderer, tE->texturePiege2[0], NULL, &rect);
        break;
    default:
        break;
    }
}

void deplacementEntite(Entite *entite, Coordonnees coordonnees, map *m, ListeEntite *listeEntite) {
    if (entite->typeEntite == UNITE) {
    }
}

void attaquerEntite(Entite *entite, Entite *cible) {
    if (entite->allegeance != cible->allegeance) {
        cible->pointsVie -= entite->pointsAttaque;
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
