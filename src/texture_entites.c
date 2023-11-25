#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/texture_entites.h"

// #include "header/base.h"

void chargerTextureEntite(texture_entite **ptE, const char *fichier, const char *fichierSelect, SDL_Renderer *renderer) {
    *ptE = malloc(sizeof(texture_entite));
    texture_entite *tE = *ptE;
    // On récupère tous les sprites de l'entite
    SDL_Surface *assetFichier = IMG_Load(fichier);
    SDL_Surface *assetFichierSelect = IMG_Load(fichierSelect);
    // SDL_Surface *assetFichierPiege = IMG_Load(fichierPiege);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = assetFichierUnite->w / TAILLE_TEXTURE_ENTITE + assetFichierPiege->w / TAILLE_TEXTURE_ENTITE;
    int nombreSurface = 4;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    SDL_Surface **resSelect = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'entité) soit affichée (on enlève les parties noires de la surface)
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,26, 32, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        resSelect[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,26, 32, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        resSelect[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        // res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
        #endif
    }
    
    tE->nbTexture = 4;
    tE->tab = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->selectionne = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->blink = SDL_CreateTextureFromSurface(renderer, res[0]);

    SDL_Rect rect = {0, 0, 32, 32};
    //SDL_Rect dstRectUnite = {0, 0, -1, -1};

    // SDL_Rect rectPiege = {0, 0, 32, 32};
    // SDL_Rect dstRectPiege = {0, 0, -1, -1};
    
    // On récupère les sprite de l'unité alliée
    for (int i = 0; i < tE->nbTexture; i++) {
        rect.x = i * 32;
        SDL_BlitSurface(assetFichier, &rect, res[i], NULL);
        tE->tab[i] = SDL_CreateTextureFromSurface(renderer, res[i]);
        SDL_QueryTexture(tE->tab[i], NULL, NULL, &rect.w, &rect.h);

        SDL_BlitSurface(assetFichierSelect, &rect, resSelect[i], NULL);
        tE->selectionne[i] = SDL_CreateTextureFromSurface(renderer, resSelect[i]);
        SDL_QueryTexture(tE->selectionne[i], NULL, NULL, &rect.w, &rect.h);
    }

    // On libère les surfaces utilisées
    SDL_FreeSurface(assetFichier);
    SDL_FreeSurface(assetFichierSelect);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
        SDL_FreeSurface(resSelect[i]);
    }
}

void chargerTextureNexus(texture_entite **ptE, const char *fichier, SDL_Renderer *renderer) {
    *ptE = malloc(sizeof(texture_entite));
    texture_entite *tE = *ptE;
    // On récupère tous les sprites de l'entite
    SDL_Surface *assetFichier = IMG_Load(fichier);
    // SDL_Surface *assetFichierPiege = IMG_Load(fichierPiege);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = assetFichierUnite->w / TAILLE_TEXTURE_ENTITE + assetFichierPiege->w / TAILLE_TEXTURE_ENTITE;
    int nombreSurface = 1;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'entité) soit affichée (on enlève les parties noires de la surface)
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,96, 96, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 96, 96, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        // res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
        #endif
    }
    
    tE->nbTexture = 1;
    tE->tab = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->selectionne = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->blink = SDL_CreateTextureFromSurface(renderer, res[0]);

    SDL_Rect rect = {0, 0, 96, 96};
    //SDL_Rect dstRectUnite = {0, 0, -1, -1};

    // SDL_Rect rectPiege = {0, 0, 32, 32};
    // SDL_Rect dstRectPiege = {0, 0, -1, -1};
    
    // On récupère les sprite de l'unité alliée
    for (int i = 0; i < tE->nbTexture; i++) {
        rect.x = i * 64;
        SDL_BlitSurface(assetFichier, &rect, res[i], NULL);
        tE->tab[i] = SDL_CreateTextureFromSurface(renderer, res[i]);
        SDL_QueryTexture(tE->tab[i], NULL, NULL, &rect.w, &rect.h);
    }

    // On libère les surfaces utilisées
    SDL_FreeSurface(assetFichier);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void chargerTexturePiege1(texture_entite **ptE, const char *fichier, SDL_Renderer *renderer) {
    *ptE = malloc(sizeof(texture_entite));
    texture_entite *tE = *ptE;
    // On récupère tous les sprites de l'entite
    SDL_Surface *assetFichier = IMG_Load(fichier);
    // SDL_Surface *assetFichierPiege = IMG_Load(fichierPiege);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = assetFichierUnite->w / TAILLE_TEXTURE_ENTITE + assetFichierPiege->w / TAILLE_TEXTURE_ENTITE;
    int nombreSurface = 2;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'entité) soit affichée (on enlève les parties noires de la surface)
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,64, 64, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        // res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
        #endif
    }
    
    tE->nbTexture = 2;
    tE->tab = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->selectionne = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->blink = SDL_CreateTextureFromSurface(renderer, res[0]);

    SDL_Rect rect = {0, 0, 64, 64};
    //SDL_Rect dstRectUnite = {0, 0, -1, -1};

    // SDL_Rect rectPiege = {0, 0, 32, 32};
    // SDL_Rect dstRectPiege = {0, 0, -1, -1};
    
    // On récupère les sprite de l'unité alliée
    for (int i = 0; i < tE->nbTexture; i++) {
        rect.x = i * 64;
        SDL_BlitSurface(assetFichier, &rect, res[i], NULL);
        tE->tab[i] = SDL_CreateTextureFromSurface(renderer, res[i]);
        SDL_QueryTexture(tE->tab[i], NULL, NULL, &rect.w, &rect.h);
    }

    // On libère les surfaces utilisées
    SDL_FreeSurface(assetFichier);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void chargerTexturePiege2(texture_entite **ptE, const char *fichier, SDL_Renderer *renderer) {
    *ptE = malloc(sizeof(texture_entite));
    texture_entite *tE = *ptE;
    // On récupère tous les sprites de l'entite
    SDL_Surface *assetFichier = IMG_Load(fichier);
    // SDL_Surface *assetFichierPiege = IMG_Load(fichierPiege);
    // On crée des surfaces de la taille d'un sprite (une surface par sprite)
    //? int nombreSurface = assetFichierUnite->w / TAILLE_TEXTURE_ENTITE + assetFichierPiege->w / TAILLE_TEXTURE_ENTITE;
    int nombreSurface = 3;
    SDL_Surface **res = malloc(sizeof(SDL_Surface*) * nombreSurface);
    // On crée les surfaces de façon à ce seulement une partie du sprite (l'entité) soit affichée (on enlève les parties noires de la surface)
    for (int i = 0; i < nombreSurface; i++) {
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE,64, 64, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
        res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        // res[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, 26, 32, 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000);
        #endif
    }
    
    tE->nbTexture = 3;
    tE->tab = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->selectionne = (SDL_Texture**) malloc(sizeof(SDL_Texture*) * tE->nbTexture);
    tE->blink = SDL_CreateTextureFromSurface(renderer, res[0]);

    SDL_Rect rect = {0, 0, 64, 64};
    //SDL_Rect dstRectUnite = {0, 0, -1, -1};

    // SDL_Rect rectPiege = {0, 0, 32, 32};
    // SDL_Rect dstRectPiege = {0, 0, -1, -1};
    
    // On récupère les sprite de l'unité alliée
    for (int i = 0; i < tE->nbTexture; i++) {
        rect.x = i * 64;
        SDL_BlitSurface(assetFichier, &rect, res[i], NULL);
        tE->tab[i] = SDL_CreateTextureFromSurface(renderer, res[i]);
        SDL_QueryTexture(tE->tab[i], NULL, NULL, &rect.w, &rect.h);
    }

    // On libère les surfaces utilisées
    SDL_FreeSurface(assetFichier);

    for (int i = 0; i < nombreSurface; i++) {
        SDL_FreeSurface(res[i]);
    }
}

void detruireTextureEntite(texture_entite *tE) {
    free(tE->tab);
    free(tE->selectionne);

    tE->nbTexture = -1;
    free(tE);
}
