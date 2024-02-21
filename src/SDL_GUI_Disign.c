#include "SDL_GUI/SDL_GUI_Disign.h"

void SDL_GUI_RoundedCorner(SDL_Texture *img, float r, SDL_Renderer *renderer, int h, int w){
    SDL_SetTextureBlendMode(img, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, img);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    //Coin en haut a gauche
    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            int dist = (r-i)*(r-i)+(r-j)*(r-j);
            if(dist >= r*r){
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }
    }
    //Coin en haut a droite
    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            int dist = (i)*(i)+(r-j)*(r-j);
            if(dist >= r*r){
                SDL_RenderDrawPoint(renderer, w-r+i, j);
            }
        }
    }
    //Coin en bas a gauche
    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            int dist = (r-i)*(r-i)+(j)*(j);
            if(dist >= r*r){
                SDL_RenderDrawPoint(renderer, i, h-r+j);
            }
        }
    }
    //Coin en bas a droite
    for(int i = 0; i < r; i++){
        for(int j = 0; j < r; j++){
            int dist = (i)*(i)+(j)*(j);
            if(dist >= r*r){
                SDL_RenderDrawPoint(renderer, w-r+i, h-r+j);
            }
        }
    }
    SDL_SetRenderTarget(renderer, NULL);
    //Fin du Corner radius
}

SDL_Texture *SDL_GUI_GenerateForm(SDL_GUI_Form form, SDL_Renderer* renderer, int w, int h, SDL_Color color, float r){
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); //texture transparente
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    switch (form){
    case SDL_GUI_Form_Rect:
        SDL_RenderFillRect(renderer, NULL);
        break;
    case SDL_GUI_Form_Ellipse:
        for(int i = 0; i < w; i++){
            for(int j = 0; j < h; j++){
                float a = w;
                float ip = i;
                float b = h;
                float jp = j;
                float x = ((a/2)-ip)*((a/2)-ip)/(a*a);
                float y = (b/2-jp)*(b/2-jp)/(b*b);
                float dist = x+y;
                if(dist <= 0.25){
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                }
                else SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
                
                SDL_RenderDrawPoint(renderer, i, j);
            }
        }

    default:
        break;
    }
    SDL_SetRenderTarget(renderer, NULL);
    SDL_GUI_RoundedCorner(texture, r, renderer, h, w);
    return texture;
}