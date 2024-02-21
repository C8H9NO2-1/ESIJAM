#include "SDL_GUI/SDL_GUI_Text.h"
#include <string.h>

SDL_GUI_Text *SDL_GUI_InitText(int x, int y, SDL_GUI_Police *police, char *text){
    SDL_GUI_Text *label = malloc(sizeof(SDL_GUI_Text));
    int tailleText = strlen(text);
    label->text = malloc(sizeof(char)*(tailleText+1));
    int i = 0;
    for(;i<tailleText+1; i++){
        label->text[i] = text[i];
    }
    strcpy(label->text, text);
    label->police = police;
    label->x = x;
    label->y = y;
    label->rate = 1.;
    return label;
}

void SDL_GUI_SetRatePrintText(SDL_GUI_Text *label, float rate){
    label->rate = rate;
}

float SDL_GUI_GetRatePrintText(SDL_GUI_Text *label){
    return label->rate;
}

void SDL_GUI_PrintText(SDL_GUI_Text *label){
    SDL_Rect rect = {label->x, label->y, 0, 0};
    char *c = label->text;
    while (*c != '\0')
    {
        if(*c >= 32 && *c <= 126){
            rect.x += rect.w;
            SDL_QueryTexture(label->police->character[*c-32], NULL, NULL, &rect.w, &rect.h);
            rect.w *= label->rate;
            rect.h *= label->rate;
            SDL_RenderCopy(label->police->renderer, label->police->character[*c-32], NULL, &rect);
        }
        else{
            rect.x += rect.w;
            SDL_QueryTexture(label->police->character[94], NULL, NULL, &rect.w, &rect.h);
            rect.w *= label->rate;
            rect.h *= label->rate;
            SDL_RenderCopy(label->police->renderer, label->police->character[94], NULL, &rect);
        }
        c++;
    }
    
}

SDL_Rect SDL_GUI_PixelSizeText(SDL_GUI_Text *label){
    SDL_Rect rect = {0, 0, 0, 0};
    SDL_Rect res = {0, 0, 0, label->police->size};
    char *c = label->text;
    while(*c != '\0'){
        if(*c >= 32 && *c <= 126){
            SDL_QueryTexture(label->police->character[*c-32], NULL, NULL, &rect.w, &rect.h);
            res.w += rect.w;
        }
        else{
            SDL_QueryTexture(label->police->character[94], NULL, NULL, &rect.w, &rect.h);
            res.w += rect.w;
        }
        c++;
    }
    return res;
}

void SDL_GUI_UpdateText(SDL_GUI_Text *label, char *text){
    free(label->text);
    int taille = strlen(text);
    label->text = malloc(sizeof(char)*(taille+1));
    strcpy(label->text, text);
}

void SDL_GUI_FreeText(SDL_GUI_Text* label){
    free(label->text);
    free(label);
}