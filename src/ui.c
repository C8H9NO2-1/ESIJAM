#include "header/base.h"

#include "header/ui.h"


void freeBouton_ui(bouton_ui *b);


ui_liste initList_ui(){
    return NULL;
}

void ajoutListe_ui(ui_liste *l, ui_type t, void *objet){
    if(*l == NULL){
        *l = malloc(sizeof(ui_maille));
        (*l)->t = t;
        (*l)->objet = objet;
        (*l)->next = NULL;
        (*l)->previous = NULL;
    }
    else{
        ui_liste li = *l;
        while(li->next != NULL){
            li = li->next;
        }
        li->next = malloc(sizeof(ui_maille));
        li->next->previous = li;
        li = li->next;
        li->next = NULL;
        li->objet = objet;
        li->t = t;
    }
    
}

void freeListe_ui(ui_liste *l){
    switch ((*l)->t)
    {
    case UI_BOUTON:
        freeBouton_ui((bouton_ui *) (*l)->objet);
        break;
    
    default:
        break;
    }

    if((*l)->previous != NULL){
        free((*l)->previous);
    }
    if((*l)->next == NULL){
        free((*l));
    }
    else{
        (*l) = (*l)->next;
    }
}




void initBouton_ui(void *liste_ui, int x, int y, int w, int h, char* text, void (*action)(void*)){
    bouton_ui *b = malloc(sizeof(bouton_ui));
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->action = action;
    b->text = text;
    //Reste 

}


void freeBouton_ui(bouton_ui *b){

}