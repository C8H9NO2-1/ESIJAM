#include "SDL_GUI/SDL_GUI_Police.h"

SDL_GUI_POLICE* SDL_GUI_InitPolice(const char* file, int size, SDL_Color color, SDL_Renderer *renderer){
    //Alocation memoire et attribution des valeurs
    SDL_GUI_POLICE* police = malloc(sizeof(SDL_GUI_POLICE));
    police->color = color;
    police->size = size;
    police->renderer = renderer;
    //Copie des textures de chaque caracteres
    TTF_Font *policeTTF = TTF_OpenFont(file, size);
    for(int i = 0; i<=126-32; i++){
        char texteTemporaire[2];
        texteTemporaire[0] = i+32;
        texteTemporaire[1] = '\0';
        SDL_Surface *s = TTF_RenderText_Blended(policeTTF, texteTemporaire, color);
        police->character[i] = SDL_CreateTextureFromSurface(police->renderer, s);
        SDL_FreeSurface(s);
    }
    TTF_CloseFont(policeTTF);
    return police;
}

void SDL_GUI_FreePolice(SDL_GUI_POLICE* police){
    for(int i = 0; i < 126-32; i++){
        SDL_DestroyTexture(police->character[i]);
    }
    free(police);
}