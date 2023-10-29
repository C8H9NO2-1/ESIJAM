#ifndef __TEXTURE_MAP_H__
#define __TEXTURE_MAP_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


#define TAILLE_TEXTURE_MAP 32

void chargeTextureMapTEMP(texture_map* tM, const char *fichier, SDL_Renderer *renderer);
void freeTextureMap(texture_map* tM);

#endif