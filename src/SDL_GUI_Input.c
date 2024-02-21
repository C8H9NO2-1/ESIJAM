#include "SDL_GUI/SDL_GUI_Input.h"
#include <string.h>

SDL_GUI_Input *SDL_GUI_InitInput(int x, int y, SDL_Texture *background, SDL_GUI_Police *police, char *text, int limitCaracters){
    //Alocation memoire et attribution des valeurs
    SDL_GUI_INPUT* obj = (SDL_GUI_Input*) malloc(sizeof(SDL_GUI_INPUT));
    obj->x = x;
    obj->y = y;
    obj->cursorPosition = -1;
    obj->police = police;
    obj->limitCaracters = limitCaracters;
    obj->rate = 1.;
    SDL_QueryTexture(background, NULL, NULL, &obj->w, &obj->h);
    obj->len = strlen(text);
    obj->text = malloc(sizeof(char)*(limitCaracters+1));
    int i = 0;
    for(; i < obj->len && i < limitCaracters; i++){
        obj->text[i] = text[i];
    }
    obj->text[i] = '\0';

    //Attribution de la texture de fond
    obj->background = SDL_CreateTexture(police->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, obj->w, obj->h);
    SDL_SetTextureBlendMode(obj->background, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(police->renderer, obj->background);
    SDL_RenderCopy(police->renderer, background, NULL, NULL);
    SDL_SetRenderTarget(police->renderer, NULL);

    return obj;
}

void SDL_GUI_SetRatePrintInput(SDL_GUI_Input *in, float rate){
    in->rate = rate;
}

float SDL_GUI_GetRatePrintInput(SDL_GUI_Input *in){
    return in->rate;
}

void SDL_GUI_PrintInput(SDL_GUI_Input *in){
    //Affichage de la texture de fond
    SDL_Rect rect = {in->x, in->y, in->w*in->rate, in->h*in->rate};
    float h = in->h*in->rate;
    float w = in->w*in->rate;
    SDL_RenderCopy(in->police->renderer, in->background, NULL, &rect);
    char *c = in->text;
    SDL_Rect src = {0, 0, 0, 0};
    SDL_Rect dst = {in->x, in->y, 0, 0};
    SDL_QueryTexture(in->police->character[*c-32], NULL, NULL, &dst.w, &dst.h);
    //Afin de ne pas depasser en hauteur, l'affichage des lettres ne doit pas deppaser la texture de fond
    if(dst.h > in->h) dst.h = in->h;
    src.h = dst.h;
    src.w = dst.w;
    dst.h *= in->rate;
    dst.w *= in->rate;
    int i = 0;
    //Affichage de chaque caractere
    while(*c != '\0' && dst.x+dst.w-in->x < w){
        //On affiche le curseur a la position voulu
        if(i == in->cursorPosition &&  (SDL_GetTicks()/100)%10 > 5){
            SDL_Color colorS;
            SDL_GetRenderDrawColor(in->police->renderer, &colorS.r, &colorS.g, &colorS.b, &colorS.a);
            SDL_SetRenderDrawColor(in->police->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(in->police->renderer, dst.x, dst.y + 0.1*h, dst.x, in->y+ 0.9*h);
            SDL_SetRenderDrawColor(in->police->renderer, colorS.r, colorS.g, colorS.b, colorS.a);
        }
        if(*c >= 32 && *c <= 126){
            SDL_RenderCopy(in->police->renderer, in->police->character[*c-32], &src, &dst);
            dst.x += dst.w;
            c++;
            SDL_QueryTexture(in->police->character[*c-32], NULL, NULL, &dst.w, &dst.h);
        }
        else{
            SDL_RenderCopy(in->police->renderer, in->police->character[94], &src, &dst);
            dst.x += dst.w;
            c++;
            SDL_QueryTexture(in->police->character[94], NULL, NULL, &dst.w, &dst.h);
        }
        if(dst.h > in->h){dst.h = in->h;}
        src.h = dst.h;
        src.w = dst.w;
        dst.h *= in->rate;
        dst.w *= in->rate;
        i++;
    }
    if(i == in->cursorPosition &&  (SDL_GetTicks()/100)%10 > 5){
        SDL_Color colorS;
        SDL_GetRenderDrawColor(in->police->renderer, &colorS.r, &colorS.g, &colorS.b, &colorS.a);
        SDL_SetRenderDrawColor(in->police->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(in->police->renderer, dst.x, dst.y + 0.1*h, dst.x, in->y+ 0.9*h);
        SDL_SetRenderDrawColor(in->police->renderer, colorS.r, colorS.g, colorS.b, colorS.a);
    }
}

void SDL_GUI_EventInput(SDL_Event *e, SDL_GUI_Input *in){
    if(e->type == SDL_KEYDOWN && in->cursorPosition >= 0){
        //Deplacement du curseur vers la droite
        if(e->key.keysym.sym == SDLK_RIGHT && in->cursorPosition < in->len){
            in->cursorPosition++;
        }
        //Deplacement du curseur vers la gauche
        else if(e->key.keysym.sym == SDLK_LEFT && in->cursorPosition > 0){
            in->cursorPosition--;
        }
        //Suppression d'un caractere a droite
        else if(e->key.keysym.sym == SDLK_DELETE && in->len > 0 && in->cursorPosition < in->len){
            for(int i = in->cursorPosition; i < in->len; i++){
                in->text[i] = in->text[i+1];
            }
            in->len--;
        }
        //Suppression d'un caractere a gauche
        else if(e->key.keysym.sym == SDLK_BACKSPACE && in->len > 0 && in->cursorPosition > 0){
            for(int i = in->cursorPosition-1; i < in->len; i++){
                in->text[i] = in->text[i+1];
            }
            in->len--;
            in->cursorPosition--;
        }
        //Ecriture d'un caractere
        else if(e->key.keysym.sym >= 32 && e->key.keysym.sym < 127 && in->limitCaracters > in->len){
            for(int i = in->len; i >= in->cursorPosition; i--){
                in->text[i+1] = in->text[i];
            }
            in->text[in->cursorPosition] = e->key.keysym.sym;
            in->len++;
            in->cursorPosition++;
        }
    }
    //Activer ou non l'ecriture
    if(e->type == SDL_MOUSEBUTTONDOWN){
        if((e->motion.x < in->x || e->motion.y < in->y || e->motion.x > in->x+in->w || e->motion.y > in->y+in->h) && in->cursorPosition >= 0){
            in->cursorPosition = -1;
        }
        else{
            in->cursorPosition = in->len;
        }
    }
}

void SDL_GUI_FreeInput(SDL_GUI_Input *in){
    free(in->text);
    SDL_DestroyTexture(in->background);
    free(in);
}