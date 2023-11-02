#include "header/base.h"

//char* generateurDePseudoMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed);

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
            if((m->tab[i + j*m->largeur] & 3) == SOL)
                SDL_RenderCopy(renderer, tM->textureSol[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else if((m->tab[i + j*m->largeur] & 3) == PLAFOND)
                SDL_RenderCopy(renderer, tM->texturePlafond[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else
                SDL_RenderCopy(renderer, tM->textureMur[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            rect.x += 31;
        }
        rect.x = 0;
        rect.y += 31;
    }
}


void afficheMapCamera(camera* cam, map *m, SDL_Renderer *renderer, texture_map *tM){
    //Recuperation des coordonnees a afficher
    int x, y, w, h;
    w = (int) ((cam->x+(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    h = (int) ((cam->y+(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    //printf(" x : %d \t y : %d \t w : %d \t h : %d \n", x, y, w, h);
    

    //Modification de la position de la camera au cas ou la camera serai parti trop loin
    if(w > m->largeur){
        cam->x = ((m->largeur)*TAILLE_TEXTURE_MAP*cam->zoom)-cam->w/2;
        w = (int) ((cam->x+(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    }
    if(h > m->hauteur){
        cam->y = ((m->hauteur)*TAILLE_TEXTURE_MAP*cam->zoom)-cam->h/2;
        h = (int) ((cam->y+(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    }
    
    x = (int) ((cam->x-(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    y = (int) ((cam->y-(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    int xp = x*TAILLE_TEXTURE_MAP - (cam->x-(cam->w/2))/cam->zoom;
    int yp = y*TAILLE_TEXTURE_MAP - (cam->y-(cam->h/2))/cam->zoom;

    //SDL_Rect tempFond = {0, 0, (w-x)*TAILLE_TEXTURE_MAP, (h-y)*TAILLE_TEXTURE_MAP};
    SDL_Rect rect = {xp, yp, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1 };
    for(int j = y; (j < h+ ( (int) 4/cam->zoom)) && (j <m->hauteur); j++){
        for(int i = x; (i < w+ ((int) 4/cam->zoom)) && (i < m->largeur); i++){
            if((m->tab[i + j*m->largeur] & 3) == SOL)
                SDL_RenderCopy(renderer, tM->textureSol[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else if((m->tab[i + j*m->largeur] & 3) == PLAFOND)
                SDL_RenderCopy(renderer, tM->texturePlafond[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else
                SDL_RenderCopy(renderer, tM->textureMur[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            rect.x += 31;
        }
        rect.x = xp;
        rect.y += 31;
    }
    SDL_RenderSetScale(renderer, cam->zoom, cam->zoom);
    //printf("Zoom : %f\n", cam->zoom);
}

void generateurDeChemin(typeU *m, int w, int h, int xBase, int yBase, int wBase, int hBase, int lengthWay, int seed){
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
    }while(m[x + y*w] == SOL);

    m[x + y*w] = SOL;
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
        m[x + y*w] = SOL;
        coince = 0;
    }
}

typeU* generateurDePseudoMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed){
    typeU* m = malloc(sizeof(typeU) * w*h);

    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            m[i + j*w] = PLAFOND;
        }
    }

    int limitYBase = yBase + hBase;
    int limitXBase = xBase + wBase;
    for(int j = yBase; j < limitYBase; j++){
        for(int i = xBase; i < limitXBase; i++){
            m[i + j*w] = SOL;
        }
    }
    int pseed = seed;
    printf("xBase : %d, xLBase : %d, yBase : %d, yLBase : %d\n", xBase, xBase+wBase, yBase, yBase+hBase);
    for(int i = 0; i < nbWay; i++){
        generateurDeChemin(m, w, h, xBase, yBase, wBase, hBase, lengthWay, pseed);
        pseed = pseed * pseed + 1;
    }

    //Detection des murs :
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            if(m[i + j*w] == PLAFOND){
                if(j > 0){
                    if (i > 0) {if(m[i-1 + (j-1)*w] == SOL){m[i + j*w] = MUR;}}
                    if(m[i + (j-1)*w] == SOL) m[i + j*w] = MUR;
                    if(i < w-1) {if(m[i+1 + (j-1)*w] == SOL) m[i + j*w] = MUR;}
                }
                if(i > 0) {if(m[i-1 + (j)*w] == SOL) m[i + j*w] = MUR;}
                if(i < w-1) {if(m[i+1 + (j)*w] == SOL) m[i + j*w] = MUR;}
                if(j < h-1){
                    if(i > 0) {if(m[i-1 + (j+1)*w] == SOL) m[i + j*w] = MUR;}
                    if(m[i + (j+1)*w] == SOL) m[i + j*w] = MUR;
                    if(i < w-1){if(m[i+1 + (j+1)*w] == SOL) m[i + j*w] = MUR;}
                }
            }
        }
    }



    /*Affichage de teste*/
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            switch (m[i + j*w])
            {
            case SOL:
                printf("_ ");
                break;

            case PLAFOND:
                printf("# ");
                break;
            case MUR:

                printf("+ ");
                break;
            
            default:
                break;
            }
            //printf("%c ", m[i + j*w]==SOL?'_':'#');
        }
        printf("\n");
    }
    return m;
}

typeU* doubleMap(typeU* map, int w, int h){
    typeU* pM = malloc(sizeof(typeU)*w*h);

    for(int j = 0; j < h/2; j++){
        for(int i = 0; i < w/2; i++){
            pM[i*2 + (j*2)*w] = map[i + j*w/2];
            pM[i*2+1 + (j*2)*w] = map[i + j*w/2];
            pM[i*2 + (j*2+1)*w] = map[i + j*w/2];
            pM[i*2+1 + (j*2+1)*w] = map[i + j*w/2];
        }
    }
    return pM;
}

map* generateurDeMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed, texture_map *tM){
    map* m = initMap(w, h);
    typeU* pM0 = generateurDePseudoMap(w/2, h/2, xBase/2, yBase/2, wBase/2, hBase/2, nbWay, lengthWay, seed);
    typeU* pM = doubleMap(pM0, w, h);
    free(pM0);

    //doubleMapTypeU();

    uint8_t murCas0[] = {4, 64, 96, 192, 224};
    uint8_t murCas1[] = {4, 2, 3, 6, 7};
    uint8_t murCas2[] = {4, 16, 20, 144, 148};
    uint8_t murCas3[] = {4, 8, 9, 40, 41};
    uint8_t murCas4[] = {1, 128};
    uint8_t murCas5[] = {1, 32};
    uint8_t murCas6[] = {1, 4};
    uint8_t murCas7[] = {1, 1};
    uint8_t murCas8[] = {8, 104, 105, 232, 233, 72, 73, 72+128, 72+128+1};
    uint8_t murCas9[] = {8, 208, 212, 240, 244, 80, 84, 80+32, 84+32};
    uint8_t murCas10[] = {8, 11, 15, 43, 47, 10, 14, 42, 46};
    uint8_t murCas11[] = {8, 22, 23, 150, 151, 18, 19, 146, 147};
    uint8_t murCas12[] = {0};
    uint8_t murCas13[] = {16, 90,91,94,95,122,123,126,127,218,219,222,223,250,251,254,255};

    uint8_t *tabMurCas[14];

    tabMurCas[0] = murCas0;
    tabMurCas[1] = murCas1;
    tabMurCas[2] = murCas2;
    tabMurCas[3] = murCas3;
    tabMurCas[4] = murCas4;
    tabMurCas[5] = murCas5;
    tabMurCas[6] = murCas6;
    tabMurCas[7] = murCas7;
    tabMurCas[8] = murCas8;
    tabMurCas[9] = murCas9;
    tabMurCas[10] = murCas10;
    tabMurCas[11] = murCas11;
    tabMurCas[12] = murCas12;
    tabMurCas[13] = murCas13;


    int oldSeed;
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            if(pM[i + j*w] == PLAFOND){
                m->tab[i + j*w] = 1 + (((unsigned int) seed%tM->nbTexturePlafond)<<2);
                
            }
            else if(pM[i + j*w] == SOL){
                m->tab[i+j*w] = 0 + (((unsigned int) seed%tM->nbTextureSol)<<2);
            }
            else if(pM[i + j*w] == MUR){
                uint8_t mur = 0;
                if(j > 0){
                    if (i > 0) {if(pM[i-1 + (j-1)*w] == SOL){mur += 1;}}
                    if(pM[i + (j-1)*w] == SOL) {mur += 2;}
                    if(i < w-1) {if(pM[i+1 + (j-1)*w] == SOL) mur += 4;}
                }
                if(i > 0) {if(pM[i-1 + (j)*w] == SOL) mur += 8;}
                if(i < w-1) {if(pM[i+1 + (j)*w] == SOL) mur += 16;}
                if(j < h-1){
                    if(i > 0) {if(pM[i-1 + (j+1)*w] == SOL) mur += 32;}
                    if(pM[i + (j+1)*w] == SOL) mur += 64;
                    if(i < w-1){if(pM[i+1 + (j+1)*w] == SOL) mur += 128;}
                }
                bool fin = false;
                for(int k = 0; (k < 14) && !fin; k++){
                    for(int l = 1; l <= tabMurCas[k][0]; l++){
                        if(mur == tabMurCas[k][l]){
                            m->tab[i+j*w] = 2 + (k<<2);
                            l = tabMurCas[k][0]+1;
                            //k = 16;
                            fin = true;
                        }
                    }
                }
            }
            oldSeed = seed;
            seed = ((unsigned int) ((seed *seed<<5)>>15));
            if(oldSeed == seed) seed = seed + 54538;
        }
    }

    free(pM);
    return m;

}