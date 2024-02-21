#ifndef __SDL_GUI_BUTTON_H__
#define __SDL_GUI_BUTTON_H__

#include "SDL_GUI_Text.h"
#include "SDL_GUI_Disign.h"

typedef struct SDL_GUI_BUTTON
{
    int x, y, w, h;
    SDL_Texture *background;
    SDL_Texture *backgroundOn;
    SDL_Texture *backgroundPrint;
    SDL_GUI_Text* text;
    float rate;
    void *argument;
    void (*call)(void *);
}SDL_GUI_BUTTON;
typedef SDL_GUI_BUTTON SDL_GUI_Button;
/**
*@brief Initialise une structure SDL_GUI_Button
*
*@param x coordonee d'abscisse du bouton
*@param y coordonee d'ordonnee du bouton
*@param w largeur du bouton
*@param h hauteur du bouton
*@param police structure SDL_GUI_Police permettent de choisir la police d'ecriture et la texture d'affichage
*@param text texte qui sera afficher sur le bouton
*@param arg est un pointeur vers le parametre de la fonction appellee lors d'un clic
*@param call est la fonction appellee lors d'un clic
*@param bgColor choix de la couleur de fond du bouton
*@param r en degre, correspond a l'arondi des quatres angles
*@param deg est le niveau de noirceur du bouton lors d'un survolement avec le curseur
*/
SDL_GUI_Button *SDL_GUI_InitButton(int x, int y, int w, int h, SDL_GUI_Police *police, char* text, void *arg, void(*call)(void *), SDL_Color bgColor, int r, int deg);
/**
 * @brief Permet de gerer les evenements du a un bouton
 * 
 * @param e dernier evenement produit
 * @param button bouton que l'on souhaite verifier les evenements
 */
void SDL_GUI_EventButton(SDL_Event* e, SDL_GUI_BUTTON *button);
/**
 * @brief Permet d'afficher un bouton
 * 
 * @param button 
 */
void SDL_GUI_PrintButton(SDL_GUI_Button *button);
/**
 * @brief Libere la memoire du au bouton
 * 
 * @param button 
 */
void SDL_GUI_FreeButton(SDL_GUI_Button *button);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param button 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintButton(SDL_GUI_Button *button, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param button 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintButton(SDL_GUI_Button *button);

#endif