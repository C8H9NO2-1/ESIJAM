#ifndef __SDL_GUI_INPUT_H__
#define __SDL_GUI_INPUT_H__

#include "SDL_GUI_Police.h"

typedef struct SDL_GUI_Input
{
    int x, y;
    int w, h;
    SDL_Texture* background;
    SDL_GUI_Police *police;
    char* text;
    int limitCaracters;
    int cursorPosition;
    int len;
    float rate;
} SDL_GUI_Input;
typedef SDL_GUI_Input SDL_GUI_INPUT;

/**
 * @brief Initialise une zone d'entree de texte
 * 
 * @param x coordonee d'abscisse de l'entree
 * @param y coordonee d'ordonnee de l'entree
 * @param background texture de fond
 * @param police structure SDL_GUI_Police permettent de choisir la police d'ecriture et la texture d'affichage
 * @param text texte par defaut
 * @param limitCaracters limite de caractere pouvant que l'utilisateur peut utiliser
 * @return SDL_GUI_Input* 
 */
SDL_GUI_Input *SDL_GUI_InitInput(int x, int y, SDL_Texture *background, SDL_GUI_Police *police, char *text, int limitCaracters);
/**
 * @brief Affiche l'entree de texte
 * 
 * @param in input
 */
void SDL_GUI_PrintInput(SDL_GUI_Input *in);
/**
 * @brief Libere en memoire touts ce qui concerne l'entree
 * 
 * @param in input
 */
void SDL_GUI_FreeInput(SDL_GUI_Input *in);
/**
 * @brief Gere les evenements du a l'entree
 * 
 * @param e dernier evenement produit
 * @param in input
 */
void SDL_GUI_EventInput(SDL_Event *e, SDL_GUI_Input *in);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param in 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintInput(SDL_GUI_Input *in, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param in 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintInput(SDL_GUI_Input *in);

#endif