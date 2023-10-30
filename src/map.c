#include "header/base.h"

char* generateurDePseudoMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed);

map* initMap(int l, int h){
    map *m = malloc(sizeof(map));
    int nbCase = l*h;
    m->tab = (uMap*) malloc(sizeof(uMap) * nbCase);
    m->largeur = l;
    m->hauteur = h;
    for(int j = 0; j < h; j++){
        for(int i = 0; i < l; i++){
            m->tab[i + j*l] = 1;
        }
    }


    /*printf("=====================================================\n");
    free(generateurDePseudoMap(50, 14, 18, 5, 7, 2, 10, 500, 25114));
    printf("=====================================================\n");*/


    return m;
}

void modifCaseMap(map *m, int x, int y, typeU tU, char texture){
    uMap *c = m->tab + x + y*m->largeur;
    *c = tU + (texture<<1);
}

void freeMap(map *m){
    free(m->tab);
    m->largeur = -1;
    m->hauteur = -1;
    free(m);
}

uMap recupCaseMap(map *m, int x, int y){
    return *(m->tab + x + y*m->largeur);
}

void afficheMap(map *m, SDL_Renderer *renderer, texture_map *tM){
    SDL_Rect rect = {0, 0, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1 };
    for(int j = 0; j < m->hauteur; j++){
        for(int i = 0; i < m->largeur; i++){
            if(m->tab[i + j*m->largeur] % 2 == 0)
                SDL_RenderCopy(renderer, tM->textureSol[(m->tab[i + j*m->largeur])>>1], NULL, &rect);
            else
                SDL_RenderCopy(renderer, tM->textureMur[(m->tab[i + j*m->largeur])>>1], NULL, &rect);
            rect.x += 31;
        }
        rect.x = 0;
        rect.y += 31;
    }
}

void generateurDeChemin(char *m, int w, int h, int xBase, int yBase, int wBase, int hBase, int lengthWay, int seed){
    //Si la seed est pair le chemin commence sur une des longueurs de la base sinon sur une hauteur
    int x;
    int y;
    int coince = 0;
    do{
        if((seed & 1) == 0){
            //Si [critere] alors le chemin est sur commence sur la longueur du haut
            x = ( (unsigned int) seed % wBase) + xBase;
            if((seed & 2) == 0)
                y = yBase -1;
            else
                y = yBase + hBase;
        }
        else{
            y = ( (unsigned int) seed % hBase) + yBase;
            if((seed & 2) == 0)
                x = xBase -1;
            else
                x = xBase + wBase;
        }
        seed = ((unsigned int) ((seed *seed<<6)>>16));
    }while(m[x + y*w] == '@');

    m[x + y*w] = '@';
    for(int i = 0; i< lengthWay; i++){
        int oldx = x;
        int oldy = y;
        int oldSeed =seed;
        do{
            x = oldx;
            y = oldy;
            if(coince > 10){
                x--;
                if(((x >= w-1) || (x <= 1) || (y >= h-1) || (y <= 1)) ||((x >= xBase - 1) && (x <= xBase + wBase) && (y > yBase - 2) && (y < yBase + hBase+1))){
                    x++;
                    x++;
                    if(((x >= w-1) || (x <= 1) || (y >= h-1) || (y <= 1)) ||((x >= xBase - 1) && (x <= xBase + wBase) && (y > yBase - 2) && (y < yBase + hBase+1))){
                        x--;
                        y--;
                        if(((x >= w-1) || (x <= 1) || (y >= h-1) || (y <= 1)) ||((x >= xBase - 1) && (x <= xBase + wBase) && (y > yBase - 2) && (y < yBase + hBase+1))){
                            y++;
                            y++;
                        }
                    }
                }
            }
            else{
            if(i < lengthWay/2){
            int act = (unsigned int) seed%4;
            switch (act)
            {
                case 0:
                    x++;
                    break;
                
                case 1:
                    x--;
                    break;

                case 2:
                    y++;
                    break;
                case 3:
                    y--;
                    break;

                default:
                    break;
            }
            }
            else{
                if ((seed & 4) == 4) y++;
                else if ((seed & 2) == 2) x--;
                else if((seed & 1) == 1) x++;
                else if((seed & 32) == 32) y--;
                else if((seed & 8) == 8) x++;
                else if((seed & 16) == 16) y++;
            }
            }
        oldSeed = seed;
        seed = ((unsigned int) ((seed *seed<<6)>>16));
        if(oldSeed == seed) seed = seed*x*y + 54538;
        coince++;
        }while(((x >= w-1) || (x <= 1) || (y >= h-1) || (y <= 1)) ||((x >= xBase - 1) && (x <= xBase + wBase) && (y > yBase - 2) && (y < yBase + hBase+1)));
        m[x + y*w] = '^';
        coince = 0;
    }
}

char* generateurDePseudoMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed){
    char* m = malloc(sizeof(char) * w*h);

    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            m[i + j*w] = '#';
        }
    }

    int limitYBase = yBase + hBase;
    int limitXBase = xBase + wBase;
    for(int j = yBase; j < limitYBase; j++){
        for(int i = xBase; i < limitXBase; i++){
            m[i + j*w] = '_';
        }
    }
    int pseed = seed;
    printf("xBase : %d, xLBase : %d, yBase : %d, yLBase : %d\n", xBase, xBase+wBase, yBase, yBase+hBase);
    for(int i = 0; i < nbWay; i++){
        generateurDeChemin(m, w, h, xBase, yBase, wBase, hBase, lengthWay, pseed);
        pseed = pseed * pseed + 1;
    }

    /*Affichage de teste*/
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            printf("%c ", m[i + j*w]);
        }
        printf("\n");
    }
    return m;
}

map* generateurDeMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed){
    map* m = initMap(w, h);
    char* pM = generateurDePseudoMap(w, h, xBase, yBase, wBase, hBase, nbWay, lengthWay, seed);

    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            if(pM[i + j*w] == '#') m->tab[i + j*w] = 1;
            else m->tab[i+j*w] = 0;
        }
    }

    free(pM);
    return m;

}