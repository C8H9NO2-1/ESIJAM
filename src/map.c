#include "header/base.h"

void initMap(map *m, int l, int h){
    int nbCase = l*h;
    m->tab = (uMap*) malloc(sizeof(uMap) * nbCase);
    m->largeur = l;
    m->hauteur = h;
    for(int i = 0; i < l; i++){
        for(int j = 0; j < h; j++){
            m->tab[i + j*h] = 3;
        }
    }
}

void modifCaseMap(map *m, int x, int y, typeU tU, char texture){
    uMap *c = m->tab + x + y*m->largeur;
    *c = tU + (texture<<1);
}

void freeMap(map *m){
    free(m->tab);
    m->largeur = -1;
    m->hauteur = -1;
}

uMap recupCaseMap(map *m, int x, int y){
    return *(m->tab + x + y*m->largeur);
}

void afficheMap(map *m, SDL_Renderer *renderer, texture_map *tM){
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1 };
    for(int i = 0; i < m->hauteur; i++){
        for(int j = 0; j < m->largeur; j++){
            printf("i : %d, j : %d\n", i, j);
            rect.x += 32;
            if(m->tab[j+i*(m->hauteur)] % 2 == 0)
                SDL_RenderCopy(renderer, tM->textureSol[(m->tab[j+i*(m->hauteur)])>>1], NULL, &rect);
            else
                SDL_RenderCopy(renderer, tM->textureMur[(m->tab[j+i*(m->hauteur)])>>1], NULL, &rect);
        }
        rect.x = 0;
        rect.y += 32;
    }
}