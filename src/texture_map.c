#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"

texture_map* chargeTextureMap(const char *fichier, SDL_Renderer *renderer){
    texture_map* tM = malloc(sizeof(texture_map));
    SDL_Surface *asset_fichier = IMG_Load(fichier);
    SDL_Surface *res = SDL_CreateRGBSurface(0, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1, 32, 0, 0, 0, 0);
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1};
    SDL_Rect dstRect = {0, 0, -1, -1};
    
    
    tM->nbTexturePlafond = 6;
    tM->nbTextureSol = 6;
    tM->nbTextureMur = 14;
    tM->textureSol = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*tM->nbTextureSol);
    tM->textureMur = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*tM->nbTextureMur);
    tM->texturePlafond = (SDL_Texture**) malloc(sizeof(SDL_Texture*)*tM->nbTexturePlafond);


    for(int i = 0; i < tM->nbTexturePlafond; i++){
        rect.x = (TAILLE_TEXTURE_MAP+1)*i;
        SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
        tM->texturePlafond[i] = SDL_CreateTextureFromSurface(renderer, res);
        SDL_QueryTexture(tM->texturePlafond[i], NULL, NULL, &rect.w, &rect.h);
    }

    rect.x = 0;
    rect.y += TAILLE_TEXTURE_MAP+1;
    for(int i = 0; i < tM->nbTextureSol; i++){
        rect.x = (TAILLE_TEXTURE_MAP+1)*i;
        SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
        tM->textureSol[i] = SDL_CreateTextureFromSurface(renderer, res);
        SDL_QueryTexture(tM->textureSol[i], NULL, NULL, &rect.w, &rect.h);
    }
    for(int i = 0; i < 3; i++){
        rect.x = 0;
        rect.y += TAILLE_TEXTURE_MAP+1;
        for(int j = 0; j < 4; j++){
            SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
            tM->textureMur[j + i*4] = SDL_CreateTextureFromSurface(renderer, res);
            SDL_QueryTexture(tM->textureMur[j + i*4], NULL, NULL, &rect.w, &rect.h);
            rect.x += TAILLE_TEXTURE_MAP+1;
        }
    }
    rect.x = 0;
    rect.y += TAILLE_TEXTURE_MAP+1;
    SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
    tM->textureMur[12] = SDL_CreateTextureFromSurface(renderer, res);
    SDL_QueryTexture(tM->textureMur[12], NULL, NULL, &rect.w, &rect.h);
    rect.x += TAILLE_TEXTURE_MAP+1;
    SDL_BlitSurface(asset_fichier, &rect, res, &dstRect);
    tM->textureMur[13] = SDL_CreateTextureFromSurface(renderer, res);
    SDL_QueryTexture(tM->textureMur[13], NULL, NULL, &rect.w, &rect.h);

    SDL_FreeSurface(res);
    SDL_FreeSurface(asset_fichier);
    return tM;
}

void freeTextureMap(texture_map* tM){
    for(int i = 0; i < tM->nbTextureMur; i++){
        SDL_DestroyTexture(tM->textureMur[i]);
    }
    for(int i = 0; i < tM->nbTexturePlafond; i++){
        SDL_DestroyTexture(tM->texturePlafond[i]);
    }
    for(int i = 0; i < tM->nbTextureSol; i++){
        SDL_DestroyTexture(tM->textureSol[i]);
    }
    free(tM->textureMur);
    free(tM->textureSol);
    free(tM->texturePlafond);
    free(tM);
}