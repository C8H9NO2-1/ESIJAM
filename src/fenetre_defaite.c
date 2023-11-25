#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"

fenetre menuDefaite(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
 
    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);
    initLabel_ui(l, para->coefResolution*LARGEUR/4, 0, para->coefResolution*LARGEUR/2, para->coefResolution*HAUTEUR*3/11, "Poulpy's last stand", renderer, font);
    initLabel_ui(l, para->coefResolution*LARGEUR/4, para->coefResolution*HAUTEUR/2, para->coefResolution*LARGEUR/2, para->coefResolution*HAUTEUR*3/11, "Defaite", renderer, font);
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
        }
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    SDL_DestroyRenderer(renderer);
    
    return fenetre_sortir;
}