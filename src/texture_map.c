#include "header/base.h"


void chargeTextureMapTEMP(texture_map* tM, const char *fichier, SDL_Renderer *renderer){
    SDL_Surface *asset_fichier = IMG_Load(fichier);
    SDL_Surface *res = SDL_CreateRGBSurface(0, 32, 32, 32, 0, 0, 0, 0);
    tM->nbTextureMur = 2;
    tM->nbTextureSol = 1;
    tM->textureSol = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*1);
    tM->textureMur = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*2);
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1};
    SDL_Rect dstRect = {0, 0, -1, -1};
    SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
    tM->textureMur[0] = SDL_CreateTextureFromSurface(renderer, res);
    rect.x = 33;
    SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
    tM->textureMur[1] = SDL_CreateTextureFromSurface(renderer, res);
    rect.x = 0;
    rect.y =33;
    SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
    tM->textureSol[0] = SDL_CreateTextureFromSurface(renderer, res);

    SDL_FreeSurface(asset_fichier);
    SDL_FreeSurface(res);

}

void freeTextureMap(texture_map* tM){
    free(tM->textureMur);
    free(tM->textureSol);
    tM->nbTextureMur = -1;
    tM->nbTextureSol = -1;
}