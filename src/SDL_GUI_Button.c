#include "SDL_GUI/SDL_GUI_Button.h"

SDL_GUI_Button *SDL_GUI_InitButton(int x, int y, int w, int h, 
    SDL_GUI_Police *police, char* text,
    void *arg, void(*call)(void *),
    SDL_Color bgColor, int r, int deg){

    //Alocation memoire et attribution des valeurs
    SDL_GUI_Button* button = malloc(sizeof(SDL_GUI_BUTTON));
    button->argument = arg;
    button->call = call;
    button->x = x;
    button->y = y;
    button->h = h;
    button->w = w;
    button->rate = 1.;

    //Attribution de la texture de fond
    button->background = SDL_CreateTexture(police->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetRenderTarget(police->renderer, button->background);
    SDL_SetRenderDrawColor(police->renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(police->renderer, NULL);
    SDL_SetRenderTarget(police->renderer, NULL);

    SDL_GUI_RoundedCorner(button->background, r, police->renderer, h, w);

    //Attribtution de la texture de survolement avec le curseur
    button->backgroundOn = SDL_CreateTexture(police->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

    SDL_SetTextureBlendMode(button->backgroundOn, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(police->renderer, button->backgroundOn);
    if(bgColor.r-deg < 0) bgColor.r = 0;
    else if(bgColor.r-deg > 255) bgColor.r =255;
    else bgColor.r = bgColor.r-deg;

    if(bgColor.g-deg < 0) bgColor.g = 0;
    else if(bgColor.g-deg > 255) bgColor.g =255;
    else bgColor.g = bgColor.g-deg;

    if(bgColor.b-deg < 0) bgColor.b = 0;
    else if(bgColor.b-deg > 255) bgColor.b =255;
    else bgColor.b = bgColor.b-deg;

    SDL_SetRenderDrawColor(police->renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    SDL_RenderFillRect(police->renderer, NULL);
    SDL_SetRenderTarget(police->renderer, NULL);

    SDL_GUI_RoundedCorner(button->backgroundOn, r, police->renderer, h, w);
    
    //Attribution du texte
    button->text = SDL_GUI_InitText(0, 0, police, text);
    button->backgroundPrint = button->backgroundOn;
    return button;
}

void SDL_GUI_SetRatePrintButton(SDL_GUI_Button *button, float rate){
    button->rate = rate;
}

float SDL_GUI_GetRatePrintButton(SDL_GUI_Button *button){
    return button->rate;
}

void SDL_GUI_PrintButton(SDL_GUI_Button *button){
    //Determination de la zone a afficher
    float w = button->w*button->rate;
    float h = button->h*button->rate;
    SDL_Rect rect = {button->x, button->y, w, h};
    SDL_RenderCopy(button->text->police->renderer, button->backgroundPrint, NULL, &rect);

    SDL_Rect sizeOfText = SDL_GUI_PixelSizeText(button->text);

    SDL_Texture *temp = SDL_CreateTexture(button->text->police->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, sizeOfText.w, sizeOfText.h);
    SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(button->text->police->renderer, temp);
    SDL_RenderClear(button->text->police->renderer);

    SDL_GUI_PrintText(button->text);
    SDL_SetRenderTarget(button->text->police->renderer, NULL);
    //Centrage du texte
    sizeOfText.w *= button->rate;
    sizeOfText.h *= button->rate;
    sizeOfText.x = button->x + (w-sizeOfText.w)/2;
    sizeOfText.y = button->y + (h-sizeOfText.h)/2;

    SDL_RenderCopy(button->text->police->renderer, temp, NULL, &sizeOfText);
    SDL_DestroyTexture(temp);
}

void SDL_GUI_EventButton(SDL_Event* e, SDL_GUI_BUTTON *button){
    float w = button->w*button->rate;
    float h = button->h*button->rate;
    if(e->motion.x > button->x && e->motion.x < w+button->x && e->motion.y > button->y && e->motion.y < button->y+h){
        button->backgroundPrint = button->backgroundOn;//Survolement du button => changement de texture
        if(e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT){
            if(button->call != NULL){
                button->call(button->argument);
            }
        }
    }
    else button->backgroundPrint = button->background;
}

void SDL_GUI_FreeButton(SDL_GUI_Button *button){
    SDL_GUI_FreeText(button->text);
    SDL_DestroyTexture(button->background);
    SDL_DestroyTexture(button->backgroundOn);
    free(button);
}