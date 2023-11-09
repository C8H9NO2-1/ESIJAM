#ifndef __TEXTURE_MAP_H__
#define __TEXTURE_MAP_H__

#define TAILLE_TEXTURE_MAP 32

struct texture_map
{
    SDL_Texture **textureSol;
    int nbTextureSol;
    SDL_Texture **textureMur;
    int nbTextureMur;
    SDL_Texture **texturePlafond;
    int nbTexturePlafond;
};
typedef struct texture_map texture_map;

texture_map* chargeTextureMap(const char *fichier, SDL_Renderer *renderer);
void freeTextureMap(texture_map* tM);

#endif