#include "header/base.h"

//========== Unité ==========

void initialiserUnite(Unite *unite, Allegeance allegeance, Coordonnees coordonnees) {
    unite->pointsVie = POINTS_DE_VIE_INITIAUX;
    unite->pointsAttaque = POINTS_ATTAQUE;
    unite->allegeance = allegeance;
    unite->coordonnees = coordonnees;
}

void detruireUnite(Unite *unite) {
    unite->pointsVie = -1;
    unite->pointsAttaque = -1;
    unite->allegeance = -1;
    unite->coordonnees.x = -1;
    unite->coordonnees.y = -1;
}

void afficherUnite(Unite *unite, SDL_Renderer *renderer, texture_unite *tU) {
    SDL_Rect rect = {unite->coordonnees.x, unite->coordonnees.y, TAILLE_TEXTURE_UNITE-1, TAILLE_TEXTURE_UNITE-1};
    if (unite->allegeance == AMI) {
        SDL_RenderCopy(renderer, tU->textureAmi[0], NULL, &rect);
    } else {
        SDL_RenderCopy(renderer, tU->textureEnnemi[0], NULL, &rect);
    }
}

//========== Piège ==========

void initialiserPiege(Piege *piege, TypePiege typePiege, Coordonnees coordonnees) {
    piege->durabilite = DURABILITE_PIEGE;
    piege->degatsInfliges = DEGATS_INFLIGES;
    piege->typePiege = typePiege;
    piege->coordonnees = coordonnees;
}

void detruirePiege(Piege *piege) {
    piege->durabilite = -1;
    piege->degatsInfliges = -1;
    piege->coordonnees.x = -1;
    piege->coordonnees.y = -1;
}

void afficherPiege(Piege *piege, SDL_Renderer *renderer, texture_piege *tP) {
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_PIEGE-1, TAILLE_TEXTURE_PIEGE-1};
    SDL_Rect dstRect = {piege->coordonnees.x, piege->coordonnees.y, -1, -1};
    if (piege->typePiege == MUR) {
        SDL_RenderCopy(renderer, tP->texturePiege1[0], &rect, &dstRect);
    } else {
        SDL_RenderCopy(renderer, tP->texturePiege2[1], &rect, &dstRect);
    }
}
