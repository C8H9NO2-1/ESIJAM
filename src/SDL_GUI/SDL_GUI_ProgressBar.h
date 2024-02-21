#ifndef __SDL_GUI_PROGRESSBAR_H__
#define __SDL_GUI_PROGRESSBAR_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef enum SDL_GUI_DirectionProgression{
    SDL_GUI_Progression_LeftToRight, SDL_GUI_Progression_RightToLeft, SDL_GUI_Progression_TopToBottom, SDL_GUI_Progression_BottomToTop
} SDL_GUI_DirectionProgression;

typedef struct SDL_GUI_ProgressBar
{
    float rate;
    float ratePrint;
    int x, y, w, h;
    SDL_Texture* bg;
    SDL_Texture* front;   
    SDL_GUI_DirectionProgression direction; 
    SDL_Renderer* renderer;
} SDL_GUI_ProgressBar;
typedef SDL_GUI_ProgressBar SDL_GUI_PROGRESSBAR;
/**
 * @brief Initialise une bar de progression
 * 
 * @param x coordonee d'abscisse de la bar de progression
 * @param y coordonee d'ordonnee de la bar de progression
 * @param w largeur de la bar de progression
 * @param h hauteur de la bar de progression
 * @param renderer rendu cible
 * @param background texture de fond
 * @param front texture de progression
 * @param direction direction de la progression
 * @return SDL_GUI_ProgressBar* 
 */
SDL_GUI_ProgressBar* SDL_GUI_InitProgressBar(int x, int y, int w, int h, SDL_Renderer* renderer, SDL_Texture* background, SDL_Texture* front, SDL_GUI_DirectionProgression direction);
/**
 * @brief Affiche la bar de progression
 * 
 * @param bar bar de progression
 */
void SDL_GUI_PrintProgressBar(SDL_GUI_ProgressBar* bar);
/**
 * @brief Modifie la valeur de progression
 * 
 * @param bar bar de progression
 * @param rate taux de progression
 */
void SDL_GUI_SetRate(SDL_GUI_ProgressBar* bar, float rate);
/**
 * @brief Libere la memoire du a la bar de progression
 * 
 * @param bar bar de progression
 */
void SDL_GUI_FreeProgressBar(SDL_GUI_ProgressBar* bar);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param bar 
 * @param rate coefficient
 */
void SDL_GUI_SetRatePrintProgressBar(SDL_GUI_ProgressBar *bar, float rate);

/**
 * @brief Modifie le coefficient de resolution de la fenetre.
 * On prend la resolution voulu et on lui applique ce coeffient pour obtenir l'interface a la taille voulu.
 * 
 * @param bar 
 * @return float coefficient
 */
float SDL_GUI_GetRatePrintProgressBar(SDL_GUI_ProgressBar *bar);

#endif