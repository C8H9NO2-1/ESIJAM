#include "header/base.h"

void chargerTextureUnite(texture_unite *tU, const char *fichier, SDL_Renderer *renderer) {
    // On récupère tous les sprites de l'unité
    SDL_Surface *asset_fichier = IMG_Load(fichier);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = asset_fichier->w / TAILLE_TEXTURE_UNITE;
    int nombreSurface = 2;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'unité) soit affichée
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,TAILLE_TEXTURE_UNITE,TAILLE_TEXTURE_UNITE, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, TAILLE_TEXTURE_UNITE, TAILLE_TEXTURE_UNITE, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        #endif
    }
    
    tU->nbTextureAmi = 1;
    tU->nbTextureEnnemi = 1;
    tU->textureAmi = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tU->nbTextureAmi);
    tU->textureEnnemi = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tU->nbTextureEnnemi);
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_UNITE - 1, TAILLE_TEXTURE_UNITE - 1};
    SDL_Rect dstRect = {0, 0, -1, -1};
    
    // On récupère le sprite de l'unité alliée
    SDL_BlitSurface(asset_fichier, &rect, res[0], &dstRect);
    tU->textureAmi[0] = SDL_CreateTextureFromSurface(renderer, res[0]);
    SDL_QueryTexture(tU->textureAmi[0], NULL, NULL, &rect.w, &rect.h);

    // On récupère le sprite de l'unité ennemie
    rect.y = TAILLE_TEXTURE_UNITE + 1; // On se déplace dans le fichier avec tous les sprites
    SDL_BlitSurface(asset_fichier, &rect, res[1], &dstRect);
    tU->textureEnnemi[0] = SDL_CreateTextureFromSurface(renderer, res[1]);
    SDL_QueryTexture(tU->textureEnnemi[0], NULL, NULL, &rect.w, &rect.h);

    // On libère les surfaces utilisées
    SDL_FreeSurface(asset_fichier);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void detruireTextureUnite(texture_unite *tU) {
    free(tU->textureAmi);
    free(tU->textureEnnemi);
    tU->nbTextureAmi = -1;
    tU->nbTextureEnnemi = -1;
}

void chargerTexturePiege(texture_piege *tP, const char *fichier, SDL_Renderer *renderer) {
    // On récupère tous les sprites de l'unité
    SDL_Surface *asset_fichier = IMG_Load(fichier);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = asset_fichier->w / TAILLE_TEXTURE_UNITE;
    int nombreSurface = 2;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'unité) soit affichée
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,TAILLE_TEXTURE_PIEGE,TAILLE_TEXTURE_PIEGE, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, TAILLE_TEXTURE_PIEGE, TAILLE_TEXTURE_PIEGE, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        #endif
    }
    
    tP->nbTexturePiege1 = 1;
    tP->nbTexturePiege2 = 1;
    tP->texturePiege1 = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tP->nbTexturePiege1);
    tP->texturePiege2 = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tP->nbTexturePiege2);
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_PIEGE - 1, TAILLE_TEXTURE_PIEGE - 1};
    SDL_Rect dstRect = {0, 0, -1, -1};
    
    // On récupère le sprite de l'unité alliée
    SDL_BlitSurface(asset_fichier, &rect, res[0], &dstRect);
    tP->texturePiege1[0] = SDL_CreateTextureFromSurface(renderer, res[0]);

    // On récupère le sprite de l'unité ennemie
    rect.y = TAILLE_TEXTURE_PIEGE + 1; // On se déplace dans le fichier avec tous les sprites
    SDL_BlitSurface(asset_fichier, &rect, res[1], &dstRect);
    tP->texturePiege2[0] = SDL_CreateTextureFromSurface(renderer, res[1]);

    // On libère les surfaces utilisées
    SDL_FreeSurface(asset_fichier);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void detruireTexturePiege(texture_piege *tP) {
    free(tP->texturePiege1);
    free(tP->texturePiege2);
    tP->nbTexturePiege1 = -1;
    tP->nbTexturePiege2 = -1;
}