#include "SDL_GUI/SDL_GUI_Slide.h"

SDL_GUI_Slide* SDL_GUI_InitSlide(int x, int y, SDL_Renderer *renderer, SDL_Texture* background, SDL_Texture* cursor, SDL_GUI_Slide_Type orientation, int deg){
    //Alocation memoire et attribution des valeurs
    SDL_GUI_Slide* slide = malloc(sizeof(SDL_GUI_Slide));
    slide->x = x;
    slide->y = y;
    slide->renderer = renderer;
    slide->orientation = orientation;
    slide->ratePrint = 1.;
    SDL_QueryTexture(background, NULL, NULL, &slide->w, &slide->h);
    SDL_QueryTexture(cursor, NULL, NULL, &slide->wCursor, &slide->hCursor);
    //printf("w : %d\th : %d\nwC : %d\thC : %d\n", slide->w, slide->h, slide->wCursor, slide->hCursor);
    slide->rate = 0.5;

    //Attribution de la texture de fond
    slide->background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, slide->w, slide->h);
    SDL_SetTextureBlendMode(slide->background, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, slide->background);
    SDL_RenderCopy(renderer, background, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    //Attribution de la texture du curseur
    slide->cursor = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, slide->wCursor, slide->hCursor);
    SDL_SetTextureBlendMode(slide->cursor, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, slide->cursor);
    SDL_RenderCopy(renderer, cursor, NULL, NULL);

    int pitch = sizeof(Uint32) * slide->wCursor;
    Uint32 *pixels = malloc(pitch * slide->hCursor);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);

    SDL_SetRenderTarget(renderer, NULL);

    //Attribution de la texture du curseur avec survolement de la souris
    slide->cursorOn = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, slide->wCursor, slide->hCursor);
    SDL_SetTextureBlendMode(slide->cursorOn, SDL_BLENDMODE_BLEND); //texture transparente

    SDL_SetTextureBlendMode(slide->cursorOn, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, slide->cursorOn);
    SDL_PixelFormat *f = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    for(int i = 0; i < slide->wCursor; i++){
        for(int j = 0;  j < slide->hCursor; j++){
            SDL_Color c;
            SDL_GetRGBA(pixels[j*slide->wCursor+i], f, &c.r, &c.g, &c.b, &c.a);
            if(c.b - deg < 0) c.b = 0;
            else if(c.b - deg > 255) c.b = 255;
            else c.b =c.b-deg;

            if(c.r - deg < 0) c.r = 0;
            else if(c.r - deg > 255) c.r = 255;
            else c.r = c.r-deg;

            if(c.g - deg < 0) c.g = 0;
            else if(c.g - deg > 255) c.g = 255;
            else c.g = c.g-deg;

            SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
            SDL_RenderDrawPoint(renderer, i, j);
        }
    }
    SDL_FreeFormat(f);
    free(pixels);
    SDL_SetRenderTarget(renderer, NULL);


    slide->cursorPrint = slide->cursorOn;
    return slide;
}

void SDL_GUI_SetRatePrintSlide(SDL_GUI_Slide *slide, float rate){
    slide->ratePrint = rate;
}

void SDL_GUI_PrintSlide(SDL_GUI_Slide* slide){
    SDL_Rect bg = {slide->x, slide->y, slide->w*slide->ratePrint, slide->h*slide->ratePrint};
    float hCursor = slide->hCursor*slide->ratePrint;
    float wCursor = slide->wCursor*slide->ratePrint;
    float w = slide->w*slide->ratePrint;
    float h = slide->h*slide->ratePrint;
    SDL_Rect cs;
    if(slide->orientation == SDL_GUI_SLIDE_TYPE_HORIZONTAL) cs = (SDL_Rect) {slide->x + slide->rate*(w - wCursor), slide->y, wCursor, hCursor};
    else if(slide->orientation == SDL_GUI_SLIDE_TYPE_VERTICAL) cs = (SDL_Rect) {slide->x, slide->y + slide->rate*(h - hCursor), wCursor, hCursor};
    SDL_RenderCopy(slide->renderer, slide->background, NULL, &bg);
    SDL_RenderCopy(slide->renderer, slide->cursorPrint, NULL, &cs);
}

void SDL_GUI_EventSlide(SDL_Event* e, SDL_GUI_Slide* slide){
    float w = slide->w*slide->ratePrint;
    float h = slide->h*slide->ratePrint;
    if(e->motion.x > slide->x && e->motion.x < slide->x + w && e->motion.y > slide->y && e->motion.y < slide->y + h){
        slide->cursorPrint = slide->cursorOn;
        if((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) && e->button.button == SDL_BUTTON_LEFT){
            if(slide->orientation == SDL_GUI_SLIDE_TYPE_HORIZONTAL) slide->rate = (e->motion.x - slide->x) / (float) (w);
            else if(slide->orientation == SDL_GUI_SLIDE_TYPE_VERTICAL) slide->rate = (e->motion.y - slide->y) / (float) (h);
        }
    }
    else slide->cursorPrint = slide->cursor;

}

void SDL_GUI_SetRateSlide(SDL_GUI_Slide* slide, float rate){
    if(rate < 0) slide->rate = 0;
    else if(rate > 1) slide->rate = 1;
    else slide->rate = rate;
}

float SDL_GUI_GetRateSlide(SDL_GUI_Slide* slide){
    return slide->rate;
}

void SDL_GUI_FreeSlide(SDL_GUI_Slide* slide){
    SDL_DestroyTexture(slide->background);
    SDL_DestroyTexture(slide->cursor);
    SDL_DestroyTexture(slide->cursorOn);
    free(slide);
}