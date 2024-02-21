#ifndef __SDL_GUI_DISIGN_H__
#define __SDL_GUI_DISIGN_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef enum SDL_GUI_Form{
    SDL_GUI_Form_Rect, SDL_GUI_Form_Ellipse
} SDL_GUI_Form;

/**
 * @brief Arrondi les coins d'une texture
 * 
 * @param img texture a qui on arrondi les angles
 * @param r  en degre, correspond a l'arondi des quatres angles
 * @param renderer rendu cible
 * @param h hauteur de la texture que l'on souhaite arrondir
 * @param w largeur de la texture que l'on souhaite arrondir
 */
void SDL_GUI_RoundedCorner(SDL_Texture *img, float r, SDL_Renderer *renderer, int h, int w);
/**
 * @brief Permet d'obtenir une forme voulu via sa largeur et sa hauteur
 * 
 * @param form forme voulu
 * @param renderer rendu cible
 * @param w largeur de la texture
 * @param h hauteur de la texture
 * @param color couleur interieur de la texture
 * @param r en degre, correspond a l'arondi des quatres angles
 * @return SDL_Texture* 
 */
SDL_Texture *SDL_GUI_GenerateForm(SDL_GUI_Form form, SDL_Renderer* renderer, int w, int h, SDL_Color color, float r);

#endif