#ifndef __MAP_H__
#define __MAP_H__
#include <stdint.h>
#include "base.h"

typedef uint8_t uMap;

struct map
{
    uMap *tab;
    int largeur;
    int hauteur;
};
typedef struct map map;

enum typeU{
    SOL, MUR
};
typedef enum typeU typeU;

struct texture_map
{
    SDL_Texture **textureSol;
    int nbTextureSol;
    SDL_Texture **textureMur;
    int nbTextureMur;
};
typedef struct texture_map texture_map;

void initMap(map *m, int l, int h);
void modifCaseMap(map *m, int x, int y, typeU tU, char texture);
void freeMap(map *m);
uMap recupCaseMap(map *m, int x, int y);
void afficheMap(map *m, SDL_Renderer *renderer, texture_map *tM);

#endif