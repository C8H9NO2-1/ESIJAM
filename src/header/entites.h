/*
    Définition et fonctions liés aux entités
*/

#ifndef __ENTITE_H__
#define __ENTITE_H__

#include "base.h"

#define POINTS_DE_VIE_INITIAUX 100
#define POINTS_ATTAQUE 10
#define DURABILITE_PIEGE 10
#define DEGATS_INFLIGES 10

//========== Coordonnées ==========
typedef struct Coordonnees Coordonnees;
struct Coordonnees {
    int x;
    int y;
};
//!==============================================================================================================================
//!========== Test entité ==========
typedef enum Allegeance Allegeance;
enum Allegeance {
    AMI, ENNEMI
};

typedef enum TypeEntite TypeEntite;
enum TypeEntite {
    UNITE, PIEGE1, PIEGE2
};

typedef struct Entite Entite;
struct Entite {
    int pointsVie;
    int pointsAttaque;
    Allegeance allegeance;
    TypeEntite typeEntite;
    Coordonnees coordonnees;
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

/**
 * @brief Fonction qui permet d'initialiser une entité
 * 
 * @param entite Pointeur vers l'entité à initialiser
 * @param allegeance Allégeance de l'entité (AMI ou ENNEMI), pour un piège cette variable vaut AMI
 * @param typeEntite Type de l'entité (UNITE, PIEGE1 ou PIEGE2)
 * @param coordonnees Coordonnées initiales de l'entité
 */
void initialiserEntite(Entite *entite, Allegeance allegeance, TypeEntite typeEntite, Coordonnees coordonnees);

/**
 * @brief Fonction qui permet de détruire une entité
 * 
 * @param entite Pointeur vers l'entité à détruire
 */
void detruireEntite(Entite *entite);

/**
 * @brief Fonction qui permet d'afficher une entité
 * 
 * @param entite Pointeur vers l'entité à afficher
 * @param renderer Pointeur vers le renderer
 * @param tE Pointeur vers les textures des entités
 */
void afficherEntite(Entite *entite, SDL_Renderer *renderer, texture_entite *tE);

/**
 * @brief Fonction qui permet le déplacement d'une entité
 * 
 * @param entite Entité à déplacer
 * @param coordonnees Destination de l'entite
 * @param m Pointeur vers la carte
 */
void deplacementEntite(Entite *entite, Coordonnees coordonnees, map *m);

/**
 * @brief Fonction qui inflige des dégâts à l'entité ciblée
 * 
 * @param entite Pointeur vers l'entité qui attaque
 * @param cible Pointeur vers l'entité qui est attaquée
 */
void attaquerEntite(Entite *entite, Entite *cible);

//!========== Fin Test ==========
//!==============================================================================================================================

// //========== Unité ==========

// typedef struct Unite Unite;
// struct Unite {
//     int pointsVie; // Cette variable peut être transformée en flottant si nécessaire
//     int pointsAttaque; // Cette variable peut être transformée en flottant si nécessaire
//     Allegeance allegeance;
//     Coordonnees coordonnees;
// };

// typedef struct texture_unite texture_unite;
// struct texture_unite {
//     SDL_Texture **textureAmi; // Tableau des textures des unités amies
//     int nbTextureAmi;
//     SDL_Texture **textureEnnemi; // Tableau des textures des unités ennemies
//     int nbTextureEnnemi; // À priori égal à nbTextureAmi
// };

// /**
//  * @brief Fonction qui permet d'initialiser une unité
//  * 
//  * @param unite Pointeur vers l'unité à initialiser
//  * @param allegeance Allégeance de l'unité (AMI ou ENNEMI)
//  * @param coordonnees Coordonnées initiales de l'unité
//  */
// void initialiserUnite(Unite *unite, Allegeance allegeance, Coordonnees coordonnees);

// /**
//  * @brief Fonction qui permet de détruire une unité
//  * 
//  * @param unite Pointeur vers l'unité à détruire
//  */
// void detruireUnite(Unite *unite);

// /**
//  * @brief Fonction qui permet d'afficher une unité
//  * 
//  * @param unite Pointeur vers l'unité à afficher
//  * @param renderer Pointeur vers le renderer
//  * @param tU Pointeur vers les textures des unités
//  */
// void afficherUnite(Unite *unite, SDL_Renderer *renderer, texture_unite *tU);

// /**
//  * @brief Fonction qui permet le déplacement d'une unité
//  * 
//  * @param unite Unité à déplacer
//  * @param coordonnees Destination de l'unité
//  * @param m Pointeur vers la carte
//  */
// void deplacementUnite(Unite *unite, Coordonnees coordonnees, map *m);

// /**
//  * @brief Fonction qui inflige des dégâts à l'unité ciblée
//  * 
//  * @param unite Pointeur vers l'unité qui attaque
//  * @param cible Pointeur vers l'unité qui est attaquée
//  */
// void attaquerUnite(Unite *unite, Unite *cible);

// //========== Piège ==========
// typedef enum TypePiege TypePiege;
// enum TypePiege {
//     PIEGE11, PIEGE21
// };

// typedef struct Piege Piege;
// struct Piege {
//     int durabilite;
//     int degatsInfliges; // Cette variable peut être transformée en flottant si nécessaire
//     TypePiege typePiege;
//     Coordonnees coordonnees;
// };

// typedef struct texture_piege texture_piege;
// struct texture_piege {
//     SDL_Texture **texturePiege1; // Tableau des textures des pièges 1 (murs cassables)
//     int nbTexturePiege1;
//     SDL_Texture **texturePiege2; // Tableau des textures des pièges 2 (à définir)
//     int nbTexturePiege2; // À priori égal à nbTexturePiege1
// };

// struct listePiege {

// };

// /**
//  * @brief Fonction qui permet d'initialiser un piège
//  * 
//  * @param piege Pointeur vers le piège à initialiser
//  * @param coordonnees Coordonnées du piège (définies par le joueur)
//  */
// void initialiserPiege(Piege *piege, TypePiege typePiege, Coordonnees coordonnees);

// /**
//  * @brief Fonction qui permet de détruire un piège
//  * 
//  * @param piege Pointeur vers le piège à détruire
//  */
// void detruirePiege(Piege *piege);

// /**
//  * @brief Fonction qui permet d'afficher un piège
//  * 
//  * @param piege Pointeur vers le piège à afficher
//  * @param renderer Pointeur vers le renderer
//  * @param tP Pointeur vers les textures des pièges
//  */
// void afficherPiege(Piege *piege, SDL_Renderer *renderer, texture_piege *tP);

// /**
//  * @brief Fonction qui diminue les points de vie d'une unité ennemie et également la durabilité d'un piège
//  * 
//  * @param piege Pointeur vers le piège qui va être activé
//  * @param unite Pointeur vers l'unité ennemie qui va être attaquée
//  */
// void activerPiege(Piege *piege, Unite *unite);

#endif