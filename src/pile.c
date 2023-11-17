#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/entites.h"
#include "header/pile.h"

Pile *initialiserPile() {
    Pile *pile = malloc(sizeof(*pile));
    pile->premier = NULL;

    return pile;
}

bool estPileVide(Pile *pile) {
    return pile->premier == NULL;
}

void empiler(Pile *pile, int nouvId) {
    ElementPile *nouveau = malloc(sizeof(*nouveau));

    nouveau->id = nouvId;
    nouveau->suivant = pile->premier;
    pile->premier = nouveau;
}

int depiler(Pile *pile) {
    int resultat = -1;

    ElementPile *elementDepile = pile->premier;

    if (!(estPileVide(pile))) {
        resultat = elementDepile->id;
        pile->premier = elementDepile->suivant;
        free(elementDepile);
    }

    return resultat;
}