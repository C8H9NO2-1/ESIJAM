#include "SDL_GUI/SDL_GUI_ButtonImg.h"

SDL_GUI_ButtonImg *SDL_GUI_InitButtonImg(int x, int y, SDL_Renderer *renderer, void *arg, void(*call)(void *), SDL_Texture *img, int r, int deg){
    int w, h;
    //Alocation memoire et attribution des valeurs
    SDL_GUI_ButtonImg* button = malloc(sizeof(SDL_GUI_BUTTONIMG));
    button->argument = arg;
    button->call = call;
    button->x = x;
    button->y = y;
    SDL_QueryTexture(img, NULL, NULL, &w, &h);
    button->h = h;
    button->w = w;
    button->rate = 1;
    button->renderer = renderer;

    //Attribution de la texture de fond
    button->background = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetRenderTarget(renderer, button->background);
    SDL_RenderCopy(renderer, img, NULL, NULL);

    int pitch = sizeof(Uint32) * w;
    Uint32 *pixels = malloc(pitch * h);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, pixels, pitch);


    SDL_SetRenderTarget(renderer, NULL);
    SDL_GUI_RoundedCorner(button->background, r, renderer, h, w);
    
    //Attribtution de la texture de survolement avec le curseur
    button->backgroundOn = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetTextureBlendMode(button->backgroundOn, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, button->backgroundOn);
    SDL_PixelFormat *f = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
    for(int i = 0; i < w; i++){
        for(int j = 0;  j < h; j++){
            SDL_Color c;
            SDL_GetRGBA(pixels[j*w+i], f, &c.r, &c.g, &c.b, &c.a);
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

    SDL_GUI_RoundedCorner(button->backgroundOn, r, renderer, h, w);

    button->backgroundPrint = button->backgroundOn;
    return button;
}

void SDL_GUI_SetRatePrintButtonImg(SDL_GUI_ButtonImg *button, float rate){
    button->rate = rate;
}

float SDL_GUI_GetRatePrintButtonImg(SDL_GUI_ButtonImg *button){
    return button->rate;
}

void SDL_GUI_PrintButtonImg(SDL_GUI_ButtonImg *button){
    float w = button->w * button->rate;
    float h = button->h * button->rate;
    SDL_Rect rect = {button->x, button->y, w, h};
    SDL_RenderCopy(button->renderer, button->backgroundPrint, NULL, &rect);
}

void SDL_GUI_EventButtonImg(SDL_Event* e, SDL_GUI_BUTTONIMG *button){
    float w = button->w * button->rate;
    float h = button->h * button->rate;
    if(e->motion.x > button->x && e->motion.x < w+button->x && e->motion.y > button->y && e->motion.y < button->y+h){
        button->backgroundPrint = button->backgroundOn;
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT){
            if(button->call != NULL){
                button->call(button->argument);
            }
        }
    }
    else button->backgroundPrint = button->background;
}

void SDL_GUI_FreeButtonImg(SDL_GUI_ButtonImg *button){
    SDL_DestroyTexture(button->background);
    SDL_DestroyTexture(button->backgroundOn);
    free(button);
}