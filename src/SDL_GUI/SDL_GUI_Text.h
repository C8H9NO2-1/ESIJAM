#ifndef __SDL_GUI_TEXT_H__
#define __SDL_GUI_TEXT_H__

#include "SDL_GUI_Police.h"

typedef struct SDL_GUI_TEXT
{
    SDL_GUI_POLICE* police;
    char *text;
    int x, y;
    float rate;
} SDL_GUI_TEXT;
typedef SDL_GUI_TEXT SDL_GUI_Text;
/**
 * @brief Initialise un texte pour l'affichage
 * 
 * @param x coordonee d'abscisse du texte
 * @param y coordonee d'ordonnee du texte
 * @param police structure SDL_GUI_Police permettent de choisir la police d'ecriture et la texture d'affichage
 * @param text texte qui sera affiche
 * @return SDL_GUI_Text* 
 */
SDL_GUI_Text *SDL_GUI_InitText(int x, int y, SDL_GUI_Police *police, char *text);
/**
 * @brief Affiche le texte
 * Pour les caracteres non lisibles, il affichera le caractere : ~
 * 
 * @param label 
 */
void SDL_GUI_PrintText(SDL_GUI_Text *label);
/**
 * @brief Met a jour le texte
 * 
 * @param label 
 * @param text nouveau texte
 */
void SDL_GUI_UpdateText(SDL_GUI_Text *label, char *text);
/**
 * @brief Recupere la taille en largeur et hauteur du texte
 * 
 * @param label 
 * @return SDL_Rect 
 */
SDL_Rect SDL_GUI_PixelSizeText(SDL_GUI_Text *label);
/**
 * @brief Libere la memoire du au texte
 * 
 * @param label 
 */
void SDL_GUI_FreeText(SDL_GUI_Text* label);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param label 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintText(SDL_GUI_Text *label, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param label 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintText(SDL_GUI_Text *label);

#endif