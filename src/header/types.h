#ifndef __TYPES_H__
#define __TYPES_H__

//========== Coordonnées ==========
typedef struct Coordonnees Coordonnees;
struct Coordonnees {
    int x;
    int y;
};

//========== Chemin ==========
typedef struct ElementCheminEnnemi ElementCheminEnnemi;
struct ElementCheminEnnemi {
    Coordonnees coordonnees;
    ElementCheminEnnemi *caseSuivante;
};

typedef struct CheminEnnemi CheminEnnemi;
struct CheminEnnemi {
    ElementCheminEnnemi *premier;
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
    // Pour les unités
    SDL_Texture **textureAmi; // Tableau des textures des unités amies
    int nbTextureAmi;
    SDL_Texture **textureEnnemi; // Tableau des textures des unités ennemies
    int nbTextureEnnemi; // À priori égal à nbTextureAmi

    // Pour les pièges
    SDL_Texture **texturePiege1; // Tableau des textures des pièges 1 (murs cassables)
    int nbTexturePiege1;
    SDL_Texture **texturePiege2; // Tableau des textures des pièges 2 (à définir)
    int nbTexturePiege2; // À priori égal à nbTexturePiege1
};

typedef struct Entite Entite;
struct Entite {
    int pointsVie;
    int pointsAttaque;
    Allegeance allegeance;
    TypeEntite typeEntite;
    Coordonnees coordonnees;
    texture_entite *texture;

    ElementCheminEnnemi *element;
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



#endif