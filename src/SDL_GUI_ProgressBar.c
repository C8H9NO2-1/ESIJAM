#include "SDL_GUI/SDL_GUI_ProgressBar.h"

SDL_GUI_ProgressBar* SDL_GUI_InitProgressBar(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture* background, SDL_Texture* front, SDL_GUI_DirectionProgression direction){
    //Alocation memoire et attribution des valeurs
    SDL_GUI_ProgressBar *bar = malloc(sizeof(SDL_GUI_PROGRESSBAR));
    bar->x = x;
    bar->y = y;
    bar->h = h;
    bar->w = w;
    bar->direction = direction;
    bar->renderer = renderer;
    bar->rate = 0;
    bar->ratePrint = 1.;
    SDL_Rect r = {0, 0, w, h};
    //Attribution de la texture de fond
    bar->bg = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(bar->bg, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, bar->bg);
    SDL_RenderCopy(renderer, background, &r, &r);
    SDL_SetRenderTarget(renderer, NULL);
    //Attribution de la texture de progression
    bar->front = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(bar->front, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, bar->front);
    SDL_RenderCopy(renderer, front, &r, &r);
    SDL_SetRenderTarget(renderer, NULL);

    return bar;
}

void SDL_GUI_SetRatePrintProgressBar(SDL_GUI_ProgressBar *bar, float rate){
    bar->ratePrint = rate;
}

float SDL_GUI_GetRatePrintProgressBar(SDL_GUI_ProgressBar *bar){
    return bar->ratePrint;
}

void SDL_GUI_PrintProgressBar(SDL_GUI_ProgressBar* bar){
    float w = bar->w * bar->ratePrint;
    float h = bar->h * bar->ratePrint;
    SDL_Rect r1;
    SDL_Rect r2 = {bar->x, bar->y, w, h};
    SDL_Rect r11;
    switch (bar->direction){
        case SDL_GUI_Progression_LeftToRight:
            r1 = (SDL_Rect) {bar->x, bar->y, (bar->rate)*(bar->w), bar->h};
            r11 = r1;
            r11.x = 0;
            r11.y = 0;
            r1.w *= bar->ratePrint;
            r1.h *= bar->ratePrint;
            break;
        case SDL_GUI_Progression_RightToLeft:
            r1 = (SDL_Rect) {bar->x + (1 - bar->rate)*(bar->w), bar->y, (bar->rate)*(bar->w), bar->h};
            r11 = r1;
            r11.y = 0;
            r11.x = (1 - bar->rate)*(bar->w);
            r1 = (SDL_Rect) {bar->x + (1 - bar->rate)*(w), bar->y, (bar->rate)*(w), h};
            break;
        case SDL_GUI_Progression_TopToBottom:
            r1 = (SDL_Rect) {bar->x, bar->y, bar->w, (bar->rate)*(bar->h)};
            r11 = r1;
            r11.x = 0;
            r11.y = 0;
            r1.w *= bar->ratePrint;
            r1.h *= bar->ratePrint;
            break;
        case SDL_GUI_Progression_BottomToTop:
            r1 = (SDL_Rect) {bar->x, bar->y + (1 - bar->rate)*(bar->h), bar->w, (bar->rate)*(bar->h)};
            r11 = r1;
            r11.x = 0;
            r11.y = (1-bar->rate)*(bar->h);
            r1 = (SDL_Rect) {bar->x, bar->y + (1 - bar->rate)*(h), w, (bar->rate)*(h)};
            break;
        default:
            break;
    }
    SDL_RenderCopy(bar->renderer, bar->bg, NULL, &r2);
    SDL_RenderCopy(bar->renderer, bar->front, &r11, &r1);
}

void SDL_GUI_SetRate(SDL_GUI_ProgressBar* bar, float rate){
    if(rate > 1) bar->rate = 1;
    else if(rate < 0) bar->rate = 0;
    else bar->rate = rate;
}

void SDL_GUI_FreeProgressBar(SDL_GUI_ProgressBar* bar){
    SDL_DestroyTexture(bar->bg);
    SDL_DestroyTexture(bar->front);
    free(bar);
}