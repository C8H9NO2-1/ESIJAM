#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"

struct argMenuPrinc{
    fenetre *sortie;
    bool *running;
};
typedef struct argMenuPrinc argMenuPrinc;

void boutonNouvellePartie(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_jeu;
}
void boutonChargerPartie(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_jeu;
}
void boutonParametre(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_parametre;
}
void boutonCredits(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_credit;
}

fenetre menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    fenetre sortie = fenetre_sortir;
 
    argMenuPrinc arg = {&sortie, &running};

    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);
    initLabel_ui(l, para->coefResolution*LARGEUR/4, 0, para->coefResolution*LARGEUR/2, para->coefResolution*HAUTEUR*3/11, "Poulpy's last stand", renderer, font);
    TTF_CloseFont(font);
    font = TTF_OpenFont("data/fonts/roboto.ttf", 200);
    initBouton_ui(l, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/3, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/8, "NOUVELLE PARTIE", boutonNouvellePartie, &arg, renderer, font);
    initBouton_ui(l, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/2, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/8, "CHARGER PARTIE", boutonChargerPartie, &arg, renderer, font);
    initBouton_ui(l, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*8/12, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/8, "OPTIONS", boutonParametre, &arg, renderer, font);
    initBouton_ui(l, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*10/12, para->coefResolution*LARGEUR/3, para->coefResolution*HAUTEUR*1/8, "CREDITS", boutonCredits, &arg, renderer, font);
    TTF_CloseFont(font);
    SDL_Event e;

    while(running){
        Uint64 frame_start = SDL_GetTicks64();

        SDL_RenderClear(renderer);
        //Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x00);
        SDL_RenderFillRect(renderer, NULL);


        afficherListe_ui(l, renderer);
        SDL_RenderPresent(renderer);

        if(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_ESCAPE:
                            running = false;
                            break;                        
                        default :
                            break;
                    }
                    break;
                default :
                    break;
            }
            eventListe_ui(l, &e);
        }
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    SDL_DestroyRenderer(renderer);
    
    return sortie;
}