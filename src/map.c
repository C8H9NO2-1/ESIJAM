#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/camera.h"
#include "header/entites.h"

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
            rect.x += TAILLE_TEXTURE_MAP-1;
        }
        rect.x = 0;
        rect.y += TAILLE_TEXTURE_MAP-1;
    }
}


void afficheMapCamera(camera* cam, map *m, SDL_Renderer *renderer, texture_map *tM, ListeEntite *lE){
    //Recuperation des coordonnees a afficher
    int x, y, w, h;
    w = (int) ((cam->x+(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    h = (int) ((cam->y+(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    
    //Modification de la position de la camera au cas ou la camera serait parti trop loin
    if(w > m->largeur-1){
        cam->x = ((m->largeur)*TAILLE_TEXTURE_MAP*cam->zoom)-cam->w/2;
        w = (int) ((cam->x+(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    }
    if(h > m->hauteur-1){
        cam->y = ((m->hauteur)*TAILLE_TEXTURE_MAP*cam->zoom)-cam->h/2;
        h = (int) ((cam->y+(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    }
    //Fin de recuperation des coordonnees a afficher
    x = (int) ((cam->x-(cam->w/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    y = (int) ((cam->y-(cam->h/2))/cam->zoom)/TAILLE_TEXTURE_MAP;
    int xp = x*TAILLE_TEXTURE_MAP - (cam->x-(cam->w/2))/cam->zoom;
    int yp = y*TAILLE_TEXTURE_MAP - (cam->y-(cam->h/2))/cam->zoom;

    SDL_Texture *fond = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (w-x+(5/cam->zoom))*TAILLE_TEXTURE_MAP, (h-y+(5/cam->zoom))*TAILLE_TEXTURE_MAP);
    SDL_SetRenderTarget(renderer, fond);

    //Ajout des textures au bonnes endroit
    SDL_Rect rect = {xp, yp, TAILLE_TEXTURE_MAP, TAILLE_TEXTURE_MAP};
    for(int j = y; (j < h+ (5/cam->zoom)) && (j <m->hauteur); j++){
        for(int i = x; (i < w+ ( 5/cam->zoom)) && (i < m->largeur); i++){
            if((m->tab[i + j*m->largeur] & 3) == SOL)
                SDL_RenderCopy(renderer, tM->textureSol[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else if((m->tab[i + j*m->largeur] & 3) == PLAFOND)
                SDL_RenderCopy(renderer, tM->texturePlafond[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            else
                SDL_RenderCopy(renderer, tM->textureMur[(m->tab[i + j*m->largeur])>>2], NULL, &rect);
            rect.x += TAILLE_TEXTURE_MAP;
        }
        rect.x = xp;
        rect.y += TAILLE_TEXTURE_MAP;
    }
    afficherListeEntite(lE, renderer, m, cam);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect dest_rect = { 0, 0, (w-x+(5/cam->zoom))*TAILLE_TEXTURE_MAP*(cam->zoom), (h-y+(5/cam->zoom))*TAILLE_TEXTURE_MAP*(cam->zoom) };
    //SDL_Rect src_rect = { 0, 0, (w-x+2)*TAILLE_TEXTURE_MAP, (h-y+2)*TAILLE_TEXTURE_MAP};
    // render
    if(SDL_RenderCopy(renderer, fond, NULL, &dest_rect) != 0) printf("%s\n", SDL_GetError());
    SDL_DestroyTexture(fond);
}


//==================================================A ne pas utiliser, fonctionne mais reste brouillon. A mettre a jour pour l'etape trois
void generateurDeChemin(typeU *m, int w, int h, int xBase, int yBase, int wBase, int hBase, int lengthWay, int seed){
    //Si la seed est pair le chemin commence sur une des longueurs de la base sinon sur une hauteur
    int x;
    int y;
    int coince = 0;
    do{
        if((seed & 1) == 0){
            //Si [critere] alors le chemin commence sur la longueur du haut
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
        seed = ((unsigned int) ((seed *seed<<6)>>16));//Mise a jour de la seed
    }while(m[x + y*w] == SOL);//On verifie que deux chemins ne commence pas au meme endroit

    m[x + y*w] = SOL;
    for(int i = 0; i< lengthWay; i++){
        //Si le chemin est bloque on le debloque en lui choisisant un chemin forcement libre
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
            //On commence par faire en sorte que le chemin s'eloigne de la base
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
            //Et que apres qu'il fasse un "pate"
            else{
                if ((seed & 4) == 4) y++;
                else if ((seed & 2) == 2) x--;
                else if((seed & 1) == 1) x++;
                else if((seed & 32) == 32) y--;
                else if((seed & 8) == 8) x++;
                else if((seed & 16) == 16) y++;
            }
            }
        //Mise à jour de la seed
        oldSeed = seed;
        seed = ((unsigned int) ((seed *seed<<6)>>16));
        if(oldSeed == seed) seed = seed*x*y + 54538;
        coince++;
        }while(((x >= w-1) || (x <= 1) || (y >= h-1) || (y <= 1)) ||((x >= xBase - 1) && (x <= xBase + wBase) && (y > yBase - 2) && (y < yBase + hBase+1)));
        m[x + y*w] = SOL;
        coince = 0;
    }
}

void ajoutMur(typeU* m, int w, int h){//Detecte les murs et les rajoute a une pseudo map
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
}

//==================================================A ne pas utiliser, fonctionne mais reste brouillon. A mettre a jour pour l'etape trois
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
    for(int i = 0; i < nbWay; i++){
        generateurDeChemin(m, w, h, xBase, yBase, wBase, hBase, lengthWay, pseed);
        pseed = pseed * pseed + 1;
    }

    //Detection des murs :
    ajoutMur(m, w, h);
    return m;
}

typeU* doubleMap(typeU* map, int w, int h){//Prend une pseudo map et double sa largeur et sa hauteur
    typeU* pM = malloc(sizeof(typeU)*w*h*4);

    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            pM[i*2 + (j*2)*w*2] = map[i + j*w];
            pM[i*2+1 + (j*2)*w*2] = map[i + j*w];
            pM[i*2 + (j*2+1)*w*2] = map[i + j*w];
            pM[i*2+1 + (j*2+1)*w*2] = map[i + j*w];
        }
    }
    return pM;
}

//Transforme une pseudo map en map
map *pseudoMapToMap(typeU *pM, texture_map *tM, int w, int h, int seed){
    map* m = initMap(w, h);

    //Liste des cas possible pour chaque texture de mur
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
                m->tab[i + j*w] = 1 + (((unsigned int) seed%tM->nbTexturePlafond)<<2);//Pour un plafond on applique une texture au hasard
                
            }
            else if(pM[i + j*w] == SOL){
                m->tab[i+j*w] = 0 + (((unsigned int) seed%tM->nbTextureSol)<<2);//Pour un sol on applique une texture au hasard
            }
            else if(pM[i + j*w] == MUR){
                //On commence par calculer l'environnement du mur
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
                //Selon l'environnement on choisi la texture (methode a optimiser)
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
            //Mise a jour pour l'aleatoire
            oldSeed = seed;
            seed = ((unsigned int) ((seed *seed<<5)>>15));
            if(oldSeed == seed) seed = seed + 54538;
        }
    }
    return m;
}

//==================================================A ne pas utiliser, fonctionne mais reste brouillon. A mettre a jour pour l'etape trois
map* generateurDeMap(int w, int h, int xBase, int yBase, int wBase, int hBase, int nbWay, int lengthWay, int seed, texture_map *tM){
    typeU* pM0 = generateurDePseudoMap(w/2, h/2, xBase/2, yBase/2, wBase/2, hBase/2, nbWay, lengthWay, seed);
    typeU* pM = doubleMap(pM0, w, h);
    free(pM0);

    map* m = pseudoMapToMap(pM, tM, w, h, seed);

    free(pM);
    return m;

}

map* lecturePseudoMap(const char* nom_fichier, texture_map* tM, int seed){
    FILE *fichier = fopen(nom_fichier, "r");
    if(fichier == NULL) exit(57468);
    //Determination de w et h
    char c = 0;
    int w = -1;
    int h = 0;
    while (c != EOF)
    {
        int i = 0;
        c =fgetc(fichier);
        while (c != EOF && c != '\n')
        {
            c = fgetc(fichier);
            if (c != 13)
                i++;
        }
        if(w == -1)
            w = i;
        else if (w != i && i != 0) exit(564);
        if(i != 0)
            h++;
        
    }
    if(w == -1 || h == 0) exit(548);

    fseek(fichier, 0, SEEK_SET);
    typeU* pseudoMap = (typeU*) malloc(sizeof(typeU)*w*h);
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            pseudoMap[i + j*w] = fgetc(fichier)-'0';
        }
        if(fgetc(fichier) == 13) fgetc(fichier);
    }
    typeU* pseudoMap2 = doubleMap(pseudoMap, w, h);
    free(pseudoMap);
    w *= 2;
    h *= 2;
    ajoutMur(pseudoMap2, w, h);

    map* m = pseudoMapToMap(pseudoMap2, tM, w, h, seed);
    free(pseudoMap2);    
    fclose(fichier);
    return m;
}

float zoomMinDetermination(map *M, parametre* para){
    float r1 = ((float) (para->coefResolution*LARGEUR/*-20*/))/((float) M->largeur*TAILLE_TEXTURE_MAP);
    float r2 = ((float) (para->coefResolution*HAUTEUR/*-20*/))/((float) M->hauteur*TAILLE_TEXTURE_MAP);
    if(r1 < r2) return r2;
    return r1;
}