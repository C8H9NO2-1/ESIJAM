/*
    Définition et fonctions liés aux entités
*/

#ifndef __ENTITE_H__
#define __ENTITE_H__

#include "base.h"

#define POINTS_DE_VIE_INITIAUX 100

//========== Coordonnées ==========
typedef struct Coordonnees Coordonnees;
struct Coordonnees {
    int x;
    int y;
};

//========== Unité ==========
typedef enum Allegeance Allegeance;
enum Allegeance {
    AMI, ENEMI
};

typedef struct Unite Unite;
struct Unite {
    int pointsVie; // Cette variable peut être transformée en flottant si nécessaire
    Allegeance allegeance;
    Coordonnees coordonnees;
};

//========== Piège ==========
typedef struct Piege Piege;
struct Piege {
    int durabilite;
    int degatsInfliges; // Cette variable peut être transformée en flottant si nécessaire
    Coordonnees coordonnees;
};


#endif