#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"

int menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
 
    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);
    initLabel_ui(l, 500, 200, 300, 150, "Poulpy's", renderer, font);
    TTF_CloseFont(font);
    font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);
    initBouton_ui(l, 100, 100, 200, 100, "Option", NULL, NULL, renderer, font);
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
                default :
                    break;
            }
            eventListe_ui(l, &e);
        }
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
        
    }

    SDL_DestroyRenderer(renderer);
    
    return 0;
}