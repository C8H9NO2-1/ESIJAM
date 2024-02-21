#ifndef __SDL_GUI_SLIDE_H__
#define __SDL_GUI_SLIDE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


typedef enum SDL_GUI_Slide_Type{
    SDL_GUI_SLIDE_TYPE_HORIZONTAL, SDL_GUI_SLIDE_TYPE_VERTICAL
}SDL_GUI_Slide_Type;

typedef struct SDL_GUI_Slide
{
    int x, y;
    int w, h;
    int wCursor, hCursor;
    SDL_GUI_Slide_Type orientation;
    float rate;
    float ratePrint;
    SDL_Texture* background;
    SDL_Texture* cursor;
    SDL_Texture* cursorOn;
    SDL_Texture* cursorPrint;
    SDL_Renderer *renderer;
}SDL_GUI_Slide;
typedef SDL_GUI_Slide SDL_GUI_SLIDE;

/**
 * @brief Initialisation d'un slider
 * 
 * @param x coordonee d'abscisse du slider
 * @param y coordonee d'ordonnee du slider
 * @param renderer rendu cible
 * @param background texture de fond
 * @param cursor texture du curseur
 * @param orientation deplacement du curseur
 * @param deg niveau de noirceur du curseur lors d'un survolement avec la souris
 * @return SDL_GUI_Slide* 
 */
SDL_GUI_Slide* SDL_GUI_InitSlide(int x, int y, SDL_Renderer *renderer, SDL_Texture* background, SDL_Texture* cursor, SDL_GUI_Slide_Type orientation, int deg);
/**
 * @brief Afficher le slider
 * 
 * @param slide 
 */
void SDL_GUI_PrintSlide(SDL_GUI_Slide* slide);
/**
 * @brief Gerer les evennements du au slider
 * 
 * @param e dernier evenement produit
 * @param slide 
 */
void SDL_GUI_EventSlide(SDL_Event* e, SDL_GUI_Slide* slide);
/**
 * @brief Met à jour le taux du slider
 * 
 * @param slide 
 * @param rate taux
 */
void SDL_GUI_SetRateSlide(SDL_GUI_Slide* slide, float rate);
/**
 * @brief Recupere la valeur du slider
 * 
 * @param slide 
 * @return float : le taux [0, 1]
 */
float SDL_GUI_GetRateSlide(SDL_GUI_Slide* slide);
/**
 * @brief Libere la memoire du au slider
 * 
 * @param slide 
 */
void SDL_GUI_FreeSlide(SDL_GUI_Slide* slide);


/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param slide 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintSlide(SDL_GUI_Slide *slide, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param slide 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintSlide(SDL_GUI_Slide *slide);

#endif