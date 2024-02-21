#ifndef __SDL_GUI_POLICE_H__
#define __SDL_GUI_POLICE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct SDL_GUI_POLICE
{
    //Contient les caractere ascii du 32(SPACE) à 126(~).
    SDL_Texture *character[126-32+1];
    int size;
    SDL_Color color;
    SDL_Renderer *renderer;
} SDL_GUI_POLICE;
typedef SDL_GUI_POLICE SDL_GUI_Police;

/**
 * @brief Initialise une police d'ecriture pour les objets de SDL_GUI
 * 
 * @param file lien d'acces au fichier de police TTF
 * @param size taille des caracteres (la hauteur en pixels)
 * @param color couleur d'ecriture
 * @param renderer pointeur de rendu
 * @return SDL_GUI_POLICE* 
 */
SDL_GUI_POLICE* SDL_GUI_InitPolice(const char* file, int size, SDL_Color color, SDL_Renderer *renderer);
/**
 * @brief Libere la place en memoire concernant cette police
 * 
 * @param police 
 */
void SDL_GUI_FreePolice(SDL_GUI_POLICE* police);


#endif