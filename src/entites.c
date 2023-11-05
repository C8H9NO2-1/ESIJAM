#include "header/base.h"

//1========== Entité ==========

void initialiserEntite(Entite *entite, Allegeance allegeance, TypeEntite typeEntite, Coordonnees coordonnees) {
    // On initialise les variables qui ne dépendent pas du type d'entite avec lequel on travaille
    entite->coordonnees = coordonnees;
    entite->typeEntite = typeEntite;

    // Si l'entite est une unite
    if (typeEntite == UNITE) {
        entite->pointsVie = POINTS_DE_VIE_INITIAUX;
        entite->pointsAttaque = POINTS_ATTAQUE;
        entite->allegeance = allegeance;
    } else { // Si l'entite est un piege
        entite->pointsVie = DURABILITE_PIEGE;
        entite->pointsAttaque = DEGATS_INFLIGES;
        entite->allegeance = AMI;
    }
}

void detruireEntite(Entite *entite) {
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
