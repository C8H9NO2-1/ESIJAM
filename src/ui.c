#include "header/base.h"

#include "header/ui.h"


void freeBouton_ui(bouton_ui *b);
void afficheBouton_ui(SDL_Renderer *renderer, bouton_ui *b);


ui_liste* initList_ui(){
    ui_liste *l;
    l = malloc(sizeof(ui_liste));
    *l = NULL;
    return l;
}

void ajoutListe_ui(ui_liste *l, ui_type t, void *objet){
    if(*l == NULL){
        *l = malloc(sizeof(ui_maille));
        (*l)->t = t;
        (*l)->objet = objet;
        (*l)->next = NULL;
    }
    else{
        ui_liste li = *l;
        while(li->next != NULL){
            li = li->next;
        }
        li->next = malloc(sizeof(ui_maille));
        li = li->next;
        li->next = NULL;
        li->objet = objet;
        li->t = t;
    }
    
}

void afficherListe_ui(ui_liste *l, SDL_Renderer *renderer){
    ui_liste li = (*l);
    while(li != NULL){
        switch(li->t){

            case UI_BOUTON:
                afficheBouton_ui(renderer, (bouton_ui *) li->objet);
                break;

            default:
                break;
        }
        li = li->next;
    }
}

void freeListe_ui(ui_liste *l){
    // switch ((*l)->t)
    // {
    // case UI_BOUTON:
    //     freeBouton_ui((bouton_ui *) (*l)->objet);
    //     break;
    
    // default:
    //     break;
    // }
    // while((*l) != NULL){
    //     ui_liste li = (*l)->next;
    //     free(*l);
    //     (*l) = li;
    // }
}

void initBouton_ui(ui_liste *l, int x, int y, int w, int h, const char* text, void (*action)(void*), SDL_Renderer *renderer, TTF_Font *f){
    bouton_ui *b = malloc(sizeof(bouton_ui));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->action = action;
    b->text = text;
    //Reste 
    SDL_Rect bigRect = {0, 0, w, h};
    SDL_Rect rect = {2, 2, w-4, h-4};
    SDL_Rect rect2 = {w/10, h/10, 4*w/5, 4*h/5};
    SDL_Surface *sText = TTF_RenderText_Blended(f, text, (SDL_Color) {255, 255, 255, 255});
    b->t_nothing = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, b->t_nothing);
    SDL_SetRenderDrawColor(renderer, 144, 144, 144, 255);
    SDL_RenderFillRect(renderer, &bigRect);
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &rect2);
    SDL_DestroyTexture(tText);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_FreeSurface(sText);

    ajoutListe_ui(l, UI_BOUTON, b);
}

void afficheBouton_ui(SDL_Renderer *renderer, bouton_ui *b){
    SDL_Rect rect = {b->x, b->y, b->w, b->h};
    SDL_RenderCopy(renderer, b->t_nothing, NULL, &rect);
}

void freeBouton_ui(bouton_ui *b){
    free(b->t_nothing);
    free(b);
}