#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "SDL_GUI/SDL_GUI.h"

struct argMenuPrinc{
    fenetre *sortie;
    bool *running;
};
typedef struct argMenuPrinc argMenuPrinc;

void boutonPartie(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_jeu;
}
void boutonParametre(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_parametre;
}
void boutonQuitter(void* data){
    argMenuPrinc *arg = (argMenuPrinc*) data;
    *(arg->running) = false;
    *(arg->sortie) = fenetre_sortir;
}

fenetre menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    fenetre sortie = fenetre_sortir;
 
    argMenuPrinc arg = {&sortie, &running};
    SDL_Event e;

    SDL_Surface *fondS = IMG_Load("data/UI/MenuPrincipal/StaticMP.png");
    SDL_Texture* fond = SDL_CreateTextureFromSurface(renderer, fondS);
    SDL_FreeSurface(fondS);
    
    SDL_Surface *bouttonStartS = IMG_Load("data/UI/MenuPrincipal/BoutonStart.png");
    SDL_Texture* bouttonStart = SDL_CreateTextureFromSurface(renderer, bouttonStartS);
    SDL_GUI_ButtonImg* start = SDL_GUI_InitButtonImg(5.5*para->coefResolution, 2.78*para->coefResolution, renderer, &arg, boutonPartie, bouttonStart, 0, 25);
    SDL_GUI_SetRatePrintButtonImg(start, para->coefResolution/100.);
    
    SDL_Surface *bouttonOptionS = IMG_Load("data/UI/MenuPrincipal/BoutonOption.png");
    SDL_Texture* bouttonOption = SDL_CreateTextureFromSurface(renderer, bouttonOptionS);
    SDL_GUI_ButtonImg* option = SDL_GUI_InitButtonImg(5.5*para->coefResolution, 4.08*para->coefResolution, renderer, &arg, boutonParametre, bouttonOption, 0, 25);
    SDL_GUI_SetRatePrintButtonImg(option, para->coefResolution/100.);
    
    SDL_Surface *bouttonQuitterS = IMG_Load("data/UI/MenuPrincipal/BoutonQuitter.png");
    SDL_Texture* bouttonQuitter = SDL_CreateTextureFromSurface(renderer, bouttonQuitterS);
    SDL_GUI_ButtonImg* quitter = SDL_GUI_InitButtonImg(5.5*para->coefResolution, 5.38*para->coefResolution, renderer, &arg, boutonQuitter, bouttonQuitter, 0, 25);
    SDL_GUI_SetRatePrintButtonImg(quitter, para->coefResolution/100.);

    while(running){
        Uint64 frame_start = SDL_GetTicks64();

        SDL_RenderClear(renderer);
        //Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x00);
        SDL_RenderFillRect(renderer, NULL);

        SDL_RenderCopy(renderer, fond, NULL, NULL);
        SDL_GUI_PrintButtonImg(start);
        SDL_GUI_PrintButtonImg(option);
        SDL_GUI_PrintButtonImg(quitter);

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
            SDL_GUI_EventButtonImg(&e, start);
            SDL_GUI_EventButtonImg(&e, option);
            SDL_GUI_EventButtonImg(&e, quitter);
        }
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    SDL_DestroyTexture(fond);
    SDL_GUI_FreeButtonImg(start);
    SDL_GUI_FreeButtonImg(option);
    SDL_GUI_FreeButtonImg(quitter);
    SDL_DestroyRenderer(renderer);
    
    return sortie;
}