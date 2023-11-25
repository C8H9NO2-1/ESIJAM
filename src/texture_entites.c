#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/texture_entites.h"

// #include "header/base.h"

void chargerTextureEntite(texture_entite **ptE, const char *fichierUnite, const char *fichierPiege, SDL_Renderer *renderer) {
    *ptE = malloc(sizeof(texture_entite));
    texture_entite *tE = *ptE;
    // On récupère tous les sprites de l'entite
    SDL_Surface *assetFichierUnite = IMG_Load(fichierUnite);
    SDL_Surface *assetFichierPiege = IMG_Load(fichierPiege);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = assetFichierUnite->w / TAILLE_TEXTURE_ENTITE + assetFichierPiege->w / TAILLE_TEXTURE_ENTITE;
    int nombreSurface = 4;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'entité) soit affichée (on enlève les parties noires de la surface)
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,26, 30, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 30, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        #endif
    }
    
    tE->nbTextureAmi = 1;
    tE->textureAmi = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTextureAmi);

    tE->nbTextureEnnemi = 1;
    tE->textureEnnemi = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTextureEnnemi);

    tE->nbTexturePiege1 = 1;
    tE->texturePiege1 = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexturePiege1);

    tE->nbTexturePiege2 = 1;
    tE->texturePiege2  = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexturePiege2);

    SDL_Rect rectUnite = {0, 0, 26, 15};
    //SDL_Rect dstRectUnite = {0, 0, -1, -1};

    SDL_Rect rectPiege = {0, 0, 26, 30};
    SDL_Rect dstRectPiege = {0, 0, -1, -1};
    
    // On récupère les sprite de l'unité alliée
    SDL_BlitSurface(assetFichierUnite, NULL, res[0], NULL);
    tE->textureAmi[0] = SDL_CreateTextureFromSurface(renderer, res[0]);
    SDL_QueryTexture(tE->textureAmi[0], NULL, NULL, &rectUnite.w, &rectUnite.h);

    // On récupère les sprite de l'unité ennemie
    //rectUnite.y = TAILLE_TEXTURE_ENTITE + 1; // On se déplace dans le fichier avec tous les sprites
    SDL_BlitSurface(assetFichierUnite, &rectUnite, res[1], NULL);
    tE->textureEnnemi[0] = SDL_CreateTextureFromSurface(renderer, res[1]);
    SDL_QueryTexture(tE->textureEnnemi[0], NULL, NULL, &rectUnite.w, &rectUnite.h);

    // On récupère les sprite du piège 1
    SDL_BlitSurface(assetFichierPiege, &rectPiege, res[2], &dstRectPiege);
    tE->texturePiege1[0] = SDL_CreateTextureFromSurface(renderer, res[2]);
    SDL_QueryTexture(tE->texturePiege1[0], NULL, NULL, &rectPiege.w, &rectPiege.h);

    // On récupère les sprite du piège 2
    rectPiege.y = TAILLE_TEXTURE_ENTITE + 1; // On se déplace dans le fichier avec tous les sprites
    SDL_BlitSurface(assetFichierPiege, &rectPiege, res[3], &dstRectPiege);
    tE->texturePiege2[0] = SDL_CreateTextureFromSurface(renderer, res[3]);
    SDL_QueryTexture(tE->texturePiege2[0], NULL, NULL, &rectPiege.w, &rectPiege.h);

    // On libère les surfaces utilisées
    SDL_FreeSurface(assetFichierUnite);
    SDL_FreeSurface(assetFichierPiege);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void detruireTextureEntite(texture_entite *tE) {
    free(tE->textureAmi);
    free(tE->textureEnnemi);
    free(tE->texturePiege1);
    free(tE->texturePiege2);

    tE->nbTextureAmi = -1;
    tE->nbTextureEnnemi = -1;
    tE->nbTexturePiege1 = -1;
    tE->nbTexturePiege2 = -1;
    free(tE);
}
