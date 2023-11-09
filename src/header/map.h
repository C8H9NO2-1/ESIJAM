#ifndef __MAP_H__
#define __MAP_H__

typedef uint8_t uMap;

struct map
{
    uMap *tab;
    int largeur;
    int hauteur;
};
typedef struct map map;

enum typeU{
    SOL, PLAFOND, MUR
};
typedef enum typeU typeU;

map* initMap(int l, int h);
void modifCaseMap(map *m, int x, int y, typeU tU, char texture);
void freeMap(map *m);
uMap recupCaseMap(map *m, int x, int y);
void afficheMap(map *m, SDL_Renderer *renderer, texture_map *tM);
map* generateurDeMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed, texture_map *tM);
void afficheMapCamera(camera* cam, map *m, SDL_Renderer *renderer, texture_map *tM);

#endif