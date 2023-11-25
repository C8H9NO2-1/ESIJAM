#include "header/base.h"

listeFin initListeFin(){
    return NULL;
}

void ajouteListeFin(listeFin *l, bool* valeur){
    if(*l == NULL){
        (*l) = malloc(sizeof(mailleFin));
        (*l)->next = NULL;
        (*l)->valeur = valeur;
    }
    else{
        listeFin li = (*l);
        while(li->next != NULL)
            li = li->next;
        li->next = malloc(sizeof(mailleFin));
        li = li->next;
        li->next = NULL;
        li->valeur = valeur;
    }
}

void supprimeListeFin(listeFin l){
    while(l != NULL){
        while(!(*(l->valeur))){
            SDL_Delay(10);
        }
        listeFin li = l->next;
        free(l->valeur);
        free(l);
        l = li;
    }
}