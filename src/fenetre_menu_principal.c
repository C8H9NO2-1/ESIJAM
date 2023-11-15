#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"

//Argument pour le second thread
struct argAfficheVideo{
  bool *running;
  parametre *para;
  camera *cam;
  SDL_Renderer *renderer;
  texture_map *tM;
  map *M;
  bool *fin;
};
typedef struct argAfficheVideo argAfficheVideo;
//Fonction qui est executer dans un autre thread que le principal et qui permet d'afficher selon des FPS
void *afficheVideo(void *data){
  argAfficheVideo *arg = (argAfficheVideo*) data;
  bool *running = arg->running;
  camera *cam = arg->cam;
  map *M = arg->M;
  texture_map *tM = arg->tM;
  parametre *para = arg->para;
  SDL_Renderer *renderer = arg->renderer;
  bool* fin = arg->fin;

  while(*running) {
        Uint64 frame_start = SDL_GetTicks64();
        
        SDL_Event e;

        //Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x00);

        SDL_RenderFillRect(renderer, NULL);

        afficheMapCamera(cam, M, renderer, tM);
        SDL_RenderPresent(renderer);

        while(SDL_PollEvent(&e)){
          if(e.type == SDL_QUIT) running = false;
        }

        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }
    *fin = true;
    return NULL;
}

int menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    bool fin = false;
  camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, 2., LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);
  texture_map* tM;
  tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

  map *M;
  M = lecturePseudoMap("data/map/pseudoMap1.pm", tM, 586);

  //Execution du second thread pour la video
  argAfficheVideo arg = {&running, para, &cam, renderer, tM, M, &fin};
  pthread_t threadVideo;
  pthread_create(&threadVideo, NULL, afficheVideo, &arg);

  SDL_Event e;
  while(running){
    SDL_WaitEvent(&e);
    switch (e.type)
    {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      switch (e.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        running = false;
        break;
      
      case SDLK_g:
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        break;
      
      case SDLK_s:
        SDL_SetWindowFullscreen(window, 0);
        break;

      default:
        controlCam(&cam, 20, 0.05, &e, 1);
        break;
      }
    break;
    
    default:
      break;
    }
    }
    //Attente de fin d'execution du second thread
    while(fin != true) SDL_Delay(50);

    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return 0;
}