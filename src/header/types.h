#ifndef __TYPES_H__
#define __TYPES_H__

typedef uint8_t uMap;
struct map
{
    uMap *tab;
    int largeur;
    int hauteur;
};
typedef struct map map;

//========== Coordonnées ==========
typedef struct Coordonnees Coordonnees;
struct Coordonnees {
    int x;
    int y;
};

//========== Chemin ==========
typedef struct ElementChemin ElementChemin;
struct ElementChemin {
    Coordonnees coordonnees;
    ElementChemin *caseSuivante;
};

typedef struct CheminAmi CheminAmi;
struct CheminAmi {
    ElementChemin *premier;
    bool valide; // Utile pour savoir si l'on peut envoyer une unité sur ce chemin ou non
};

typedef struct CheminEnnemi CheminEnnemi;
struct CheminEnnemi {
    ElementChemin *premier;
};

typedef struct ListeCheminsEnnemis ListeCheminsEnnemis;
struct ListeCheminsEnnemis {
    CheminEnnemi *chemin1;
    CheminEnnemi *chemin2;
    CheminEnnemi *chemin3;
    CheminEnnemi *chemin4;
};

//========== Entités ==========
typedef enum Allegeance Allegeance;
enum Allegeance {
    AMI, ENNEMI
};

typedef enum TypeEntite TypeEntite;
enum TypeEntite {
    UNITE, PIEGE1, PIEGE2
};

typedef struct texture_entite texture_entite;
struct texture_entite {
    SDL_Texture **tab; // Tableau des textures
    SDL_Texture *blink; // Texture de l'entité qui clignote
    SDL_Texture **selectionne; // Tableau des textures de l'entité si celle-ci est sélectionnée par le joueur
    int nbTexture;
};

typedef struct Entite Entite;
struct Entite {
    int pointsVie;
    int pointsAttaque;
    Allegeance allegeance;
    TypeEntite typeEntite;
    Coordonnees coordonnees;
    texture_entite *texture;
    int indiceTexture;
    bool blink;
    bool selectionne;

    // Nécessaires pour les unités alliées
    bool nouvelObjectif;
    Coordonnees objectif;

    ElementChemin *element;
};

// Dans la première case du dernier tableau on met les unités et dans la deuxième les pièges
typedef struct ListeEntite ListeEntite;
struct ListeEntite {
    Entite ****entites;
};

//========== Pile ==========
typedef struct ElementPile ElementPile;
struct ElementPile {
    int id;
    ElementPile *suivant;
};

typedef struct Pile Pile;
struct Pile {
    ElementPile *premier;
};

//========== Graphe ==========
typedef struct Graphe Graphe;
struct Graphe {

    int nbreLigne;

    int** matriceAdjacenceEnemi1;
    int** matriceAdjacenceEnemi2; // Cette matrice est la même que celle de l'enemi 1 sauf que l'on ne tient pas compte des murs cassables
    int** matriceAdjacenceAllie;
};

//=========Liste Fin Thread=======
typedef struct mailleFin mailleFin;
typedef mailleFin* listeFin;
struct mailleFin{
    bool* valeur;
    listeFin next;
};

struct argUniteAllie {
    bool *running;
    Entite *entite;
    ListeEntite *listeEntite;
    map *m;
    bool *defeat;
    Graphe *graphe;
    bool *exist;
};
typedef struct argUniteAllie argUniteAllie;

#endif