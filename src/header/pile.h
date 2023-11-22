#ifndef __PILE_H__
#define __PILE_H__

Pile *initialiserPile();

bool estPileVide(Pile *pile);

void empiler(Pile *pile, int nouvId);

int depiler(Pile *pile);

#endif