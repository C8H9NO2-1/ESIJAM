#ifndef __PILE_H__
#define __PILE_H__

typedef struct ElementPile ElementPile;
struct ElementPile {
    int id;
    ElementPile *suivant;
};

typedef struct Pile Pile;
struct Pile {
    ElementPile *premier;
};

Pile *initialiserPile();

bool estPileVide(Pile *pile);

void empiler(Pile *pile, int nouvId);

int depiler(Pile *pile);

#endif