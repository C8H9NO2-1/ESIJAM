#include "header/base.h"

int menuPrincipal(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    //float time = 0;

    while(running) {
        Uint64 frame_start = SDL_GetTicks64();
        //time = (float)frame_start / 1000;

        // Exemple : remplir la fenêtre bleu pâle (rouge=80 vert=160 bleu=255).
        SDL_SetRenderDrawColor(renderer, 80, 160, 255, 0x00);
        SDL_RenderFillRect(renderer, NULL);

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
                
                
                
            }
        }
        //const Uint8 *kbd_state = SDL_GetKeyboardState(NULL);
        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }
    SDL_DestroyRenderer(renderer);
    return 0;
}