/*
    Fonctions liées aux textures des entités du jeu
*/

#ifndef __TEXTURE_ENTITES_H__
#define __TEXTURE_ENTITES_H__

#define TAILLE_TEXTURE_ENTITE 64

// #define TAILLE_TEXTURE_UNITE 32
// #define TAILLE_TEXTURE_PIEGE 32

/**
 * @brief Fonction qui permet de charger toutes les textures des entités
 * 
 * @param tU Pointeur vers la structure contenant les textures des entitèr
 * @param fichier Fichier contenant toutes les textures des entitès
 * @param renderer Pointeur vers le renderer
 */
void chargerTextureEntite(texture_entite **ptE, const char *fichier, const char *fichierSelect, SDL_Renderer *renderer); //! Il sera peut être nécessaire de passer un seul fichier en paramètre, dépendant de comment Tym fait les fichiers

/**
 * @brief Fonction qui permet de détruire toutes les textures des entités
 * 
 * @param tE Pointeur vers la structure contenant les textures des entités
 */
void detruireTextureEntite(texture_entite *tE);

#endif