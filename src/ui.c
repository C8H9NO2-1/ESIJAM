#include "header/base.h"

#include "header/ui.h"


void freeBouton_ui(bouton_ui *b);
void afficheBouton_ui(SDL_Renderer *renderer, bouton_ui *b);
void eventBouton_ui(bouton_ui *b, SDL_Event *e);
void freeLabel_ui(label_ui *la);
void afficheLabel_ui(SDL_Renderer *renderer, label_ui *la);
 

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

            case UI_LABEL:
                afficheLabel_ui(renderer, (label_ui *) li->objet);
                break;

            default:
                break;
        }
        li = li->next;
    }
}

void eventListe_ui(ui_liste *l, SDL_Event *e){
    ui_liste li = (*l);
    while(li != NULL){
        switch(li->t){

            case UI_BOUTON:
                eventBouton_ui((bouton_ui *) li->objet, e);
                break;

            default:
                break;
        }
        li = li->next;
    }
}

void freeListe_ui(ui_liste *l){
    while((*l) != NULL){

        switch ((*l)->t)
        {
        case UI_BOUTON:
            freeBouton_ui((bouton_ui *) (*l)->objet);
            break;
        
        case UI_LABEL:
            freeLabel_ui((label_ui *) (*l)->objet);
            break;
        
        default:
            break;
        }
        ui_liste li = (*l)->next;
        free(*l);
        (*l) = li;
    }
}
void initLabel_ui(ui_liste *l, int x, int y, int w, int h, const char* text, SDL_Renderer *renderer, TTF_Font *f){
    label_ui *la = malloc(sizeof(label_ui));
    la->x = x;
    la->y = y;
    la->w = w;
    la->h = h;
    la->text = text;

    la->t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_Surface *sText = TTF_RenderText_Blended(f, text, (SDL_Color) {255, 255, 255, 255});
    SDL_Texture *tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_FreeSurface(sText);
    SDL_SetRenderTarget(renderer, la->t);
    SDL_Rect r = {0, 0, w, h};
    SDL_RenderCopy(renderer, tText, NULL, &r);
    SDL_DestroyTexture(tText);
    
    SDL_SetRenderTarget(renderer, NULL);
    ajoutListe_ui(l, UI_LABEL, la);
}

void afficheLabel_ui(SDL_Renderer *renderer, label_ui *la){
    SDL_Rect r = {la->x, la->y, la->w, la->h};
    SDL_RenderCopy(renderer, la->t, NULL, &r);
}

void freeLabel_ui(label_ui *la){
    SDL_DestroyTexture(la->t);
    free(la);
}

void initBouton_ui(ui_liste *l, int x, int y, int w, int h, const char* text, void (*action)(void*), void* data, SDL_Renderer *renderer, TTF_Font *f){
    bouton_ui *b = malloc(sizeof(bouton_ui));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->action = action;
    b->text = text;
    b->data = data;
    //Reste 
    SDL_Rect bigRect = {0, 0, w, h};
    SDL_Rect rect = {2, 2, w-4, h-4};
    SDL_Rect rect2 = {w/10, h/10, 4*w/5, 4*h/5};
    SDL_Surface *sText = TTF_RenderText_Blended(f, text, (SDL_Color) {255, 255, 255, 255});
    
    //Generation de la premiere texture (texture quand rien ne se passe)
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
    SDL_DestroyTexture(tText);

    //Generation de la seconde texture (texture quand le curseur est sur le bouton)
    b->t_on = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, b->t_on);
    SDL_SetRenderDrawColor(renderer, 188, 188, 188, 255);
    SDL_RenderFillRect(renderer, &bigRect);
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderFillRect(renderer, &rect);

    tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &rect2);
    SDL_DestroyTexture(tText);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(tText);

    //Generation de la troisieme texture (texture quand on clique dessus)
    b->t_click = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, b->t_click);
    SDL_SetRenderDrawColor(renderer, 210, 210, 210, 255);
    SDL_RenderFillRect(renderer, &bigRect);
    SDL_SetRenderDrawColor(renderer, 130, 130, 130, 255);
    SDL_RenderFillRect(renderer, &rect);

    tText = SDL_CreateTextureFromSurface(renderer, sText);
    SDL_RenderCopy(renderer, tText, NULL, &rect2);
    SDL_DestroyTexture(tText);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(tText);

    SDL_FreeSurface(sText);

    b->t_afficher = b->t_nothing;

    ajoutListe_ui(l, UI_BOUTON, b);
}

void afficheBouton_ui(SDL_Renderer *renderer, bouton_ui *b){
    SDL_Rect rect = {b->x, b->y, b->w, b->h};
    SDL_RenderCopy(renderer, b->t_afficher, NULL, &rect);
}

void eventBouton_ui(bouton_ui *b, SDL_Event *e){
    if(e->type == SDL_MOUSEMOTION){
        if(e->motion.x>b->x && e->motion.x<(b->x+b->w)){
            if(e->motion.y>b->y && e->motion.y<(b->y+b->h))
                b->t_afficher = b->t_on;
            else
                b->t_afficher = b->t_nothing;
        }
        else
            b->t_afficher = b->t_nothing;
    }
    else if(e->type == SDL_MOUSEBUTTONDOWN){
        if(e->motion.x>b->x && e->motion.x<(b->x+b->w)){
            if(e->motion.y>b->y && e->motion.y<(b->y+b->h)){
                b->t_afficher = b->t_on;
                if(b->action != NULL)
                    b->action(b->data);
            }
        }
    }
}

void freeBouton_ui(bouton_ui *b){
    SDL_DestroyTexture(b->t_nothing);
    SDL_DestroyTexture(b->t_on);
    SDL_DestroyTexture(b->t_click);
    free(b);
}