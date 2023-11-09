#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"

int menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    //float time = 0;

  camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, 2., LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);

  texture_map* tM;
  tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

  map *M;
  M = generateurDeMap(100, 56, 48, 20, 28, 8, 10, 1000, 315, tM);

    while(running) {
        Uint64 frame_start = SDL_GetTicks64();
        //time = (float)frame_start / 1000;

        //Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x00);

        SDL_RenderFillRect(renderer, NULL);

        afficheMapCamera(&cam, M, renderer, tM);
        SDL_RenderPresent(renderer);

        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)
                running = false;
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                  running = false;
                }
                else if (e.key.keysym.sym == SDLK_e)
                {
                  SDL_SetWindowTitle(window, "SUPER !!!");
                }
                // else if(e.key.keysym.sym == SDLK_f){
                //   SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                // }
                else if(e.key.keysym.sym == SDLK_g){
                  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else if(e.key.keysym.sym == SDLK_s){
                  SDL_SetWindowFullscreen(window, 0);
                }
                else{
                  controlCam(&cam, 16, 0.2, &e);
                }
                
                
            }
        }
        //const Uint8 *kbd_state = SDL_GetKeyboardState(NULL);
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    // freeListeEntite(listeEntite);
    // freeMatriceAdjacence(matrice, *M);
    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return 0;
}