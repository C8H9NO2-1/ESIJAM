/*
    Fonctions liées aux textures des entités du jeu
*/

#ifndef __TEXTURE_ENTITES_H__
#define __TEXTURE_ENTITES_H__

#include "base.h"

#define TAILLE_TEXTURE_UNITE 32
#define TAILLE_TEXTURE_PIEGE 32

/**
 * @brief Fonction qui permet de charger toutes les textures des unités
 * 
 * @param tU Pointeur vers la structure contenant les textures des unités
 * @param fichier Fichier contenant toutes les textures des unités
 * @param renderer Pointeur vers le renderer
 */
void chargerTextureUnite(texture_unite *tU, const char *fichier, SDL_Renderer *renderer);

/**
 * @brief Fonction qui permet de détruire toutes les textures des unités
 * 
 * @param tU Pointeur vers la structure contenant les textures des unités
 */
void detruireTextureUnite(texture_unite *tU);

/**
 * @brief Fonction qui permet de charger toutes les textures des pièges
 * 
 * @param tP Pointeur vers la structure contenant les textures des pièges
 * @param fichier Fichier contenant toutes les textures des pièges
 * @param renderer Pointeur vers le renderer
 */
void chargerTexturePiege(texture_piege *tP, const char *fichier, SDL_Renderer *renderer);

/**
 * @brief Fonction qui permet de détruire toutes les textures des pièges
 * 
 * @param tP Pointeur vers la structure contenant les textures des pièges
 */
void detruireTexturePiege(texture_piege *tP);

#endif