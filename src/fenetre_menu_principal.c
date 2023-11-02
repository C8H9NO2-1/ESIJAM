#include "header/base.h"
#include "header/texture_map.h"

int menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    //float time = 0;

/*
==================================================Teste de chargement de map
*/

  camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, 2., LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);

  texture_map* tM;
  tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

  map *M;
  //M = generateurDeMap(51, 29, 24, 10, 14, 4, 10, 1000, 25114, tM);
  //M = generateurDeMap(51, 29, 24, 10, 14, 4, 10, 1000, 54, tM);
  //M = generateurDeMap(51, 29, 24, 10, 14, 4, 10, 1000, 128, tM);
  M = generateurDeMap(100, 56, 48, 20, 28, 8, 10, 1000, 315, tM);

  for(int j = 0; j < M->hauteur; j++){
    for(int i = 0; i < M->largeur; i++){
      printf("%d ", M->tab[i + j*M->largeur] && 1);
    }
    printf("\n");
  }



    while(running) {
        Uint64 frame_start = SDL_GetTicks64();
        //time = (float)frame_start / 1000;

        // Exemple : remplir la fenêtre bleu pâle (rouge=80 vert=160 bleu=255).
        SDL_SetRenderDrawColor(renderer, 80, 160, 255, 0x00);

        SDL_RenderFillRect(renderer, NULL);

        /*SDL_Rect rectTemp = { 32, 32, TAILLE_TEXTURE_MAP-1, TAILLE_TEXTURE_MAP-1 };
        SDL_RenderCopy(renderer, tM.textureSol[0], NULL, &rectTemp);*/
        //afficheMap(M, renderer, tM);
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
                else if(e.key.keysym.sym == SDLK_f){
                  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
                }
                else if(e.key.keysym.sym == SDLK_g){
                  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
                else if(e.key.keysym.sym == SDLK_s){
                  SDL_SetWindowFullscreen(window, 0);
                }
                else{
                  controlCam(&cam, 16, 0.2, &e);
                  //afficheMapCamera(&cam, M, renderer, tM);
                }
                
                
            }
        }
        //const Uint8 *kbd_state = SDL_GetKeyboardState(NULL);
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return 0;
}