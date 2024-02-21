#ifndef __SDL_GUI_BUTTONIMG_H__
#define __SDL_GUI_BUTTONIMG_H__

#include "SDL_GUI_Disign.h"

typedef struct SDL_GUI_BUTTONIMG
{
    int x, y, w, h;
    SDL_Texture *background;
    SDL_Texture *backgroundOn;
    SDL_Texture *backgroundPrint;
    SDL_Renderer *renderer;
    float rate;
    void *argument;
    void (*call)(void *);
}SDL_GUI_BUTTONIMG;
typedef SDL_GUI_BUTTONIMG SDL_GUI_ButtonImg;
/**
 * @brief Initialise une structure SDL_GUI_ButtonImg en prenant comme hauteur et largeur celle de l'image en entree
 * 
 * @param x coordonee d'abscisse du bouton
 * @param y coordonee d'ordonnee du bouton
 * @param renderer cible du rendu
 * @param arg est un pointeur vers le parametre de la fonction appellee lors d'un clic
 * @param call est la fonction appellee lors d'un clic
 * @param img image de fond du bouton
 * @param r en degre, correspond a l'arondi des quatres angles
 * @param deg est le niveau de noirceur du bouton lors d'un survolement avec le curseur
 * @return SDL_GUI_ButtonImg* 
 */
SDL_GUI_ButtonImg *SDL_GUI_InitButtonImg(int x, int y, SDL_Renderer *renderer, void *arg, void(*call)(void *), SDL_Texture *img, int r, int deg);
/**
 * @brief Permet de gerer les evenements du a un bouton
 * 
 * @param e dernier evenement produit
 * @param button bouton que l'on souhaite verifier les evenements
 */
void SDL_GUI_EventButtonImg(SDL_Event* e, SDL_GUI_ButtonImg *button);
/**
 * @brief Permet d'afficher un bouton
 * 
 * @param button 
 */
void SDL_GUI_PrintButtonImg(SDL_GUI_ButtonImg *button);
/**
 * @brief Libere la memoire du au bouton
 * 
 * @param button 
 */
void SDL_GUI_FreeButtonImg(SDL_GUI_ButtonImg *button);


/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param button 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintButtonImg(SDL_GUI_ButtonImg *button, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param button 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintButtonImg(SDL_GUI_ButtonImg *button);

#endif