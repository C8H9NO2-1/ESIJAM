#include "header/base.h"

#include "header/parametre.h"
#include "header/fenetre_menu_principal.h"
#include "header/jeu.h"
/*
Liste des erreurs :
    =1 : Le fichier de parametre (data/jeu.config) n'a pas pu etre ouvert
    =2 : Le fichier de parametre (data/jeu.config) n'est pas au format voulu
    =3 : Erreur de demarrage de la SDL
*/
int main(int argc, char *argv[]){
    int retour = 0;
    fenetre demarage = START;
    if(SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0){
        exit(3);
    }
    
    #ifndef __APPLE__
    IMG_Init(IMG_INIT_PNG);
    #endif
    TTF_Init();

    parametre para = recupererParametre();

    SDL_Window *window =
        SDL_CreateWindow("Poulpy’s last stand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR*para.coefResolution, HAUTEUR*para.coefResolution, para.typeEcran);
    Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048); 
    Mix_VolumeMusic(MIX_MAX_VOLUME);
    while(demarage){
        switch (demarage)
        {
        case fenetre_menu_principal:
            demarage = jeu(window, &para);
            break;
        case fenetre_parametre:
            //demarage = parametre(window, &para);
            break;
        case fenetre_jeu:
            demarage = jeu(window, &para);
            break;
        default:
            demarage = fenetre_sortir;
            break;
        }
    }

    Mix_HaltMusic();
    Mix_CloseAudio();
    Mix_Quit();


    SDL_DestroyWindow(window);

    TTF_Quit();
    #ifndef __APPLE__
    IMG_Quit();
    #endif
    
    SDL_Quit();

    return retour;
}