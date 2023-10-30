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

map* initMap(int l, int h);
void modifCaseMap(map *m, int x, int y, typeU tU, char texture);
void freeMap(map *m);
uMap recupCaseMap(map *m, int x, int y);
void afficheMap(map *m, SDL_Renderer *renderer, texture_map *tM);
map* generateurDeMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed);

#endif