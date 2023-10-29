#ifndef __TEXTURE_MAP_H_
#define __TEXTURE_MAP_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


#define TAILLE_TEXTURE_MAP 32

struct texture_map
{
    SDL_Texture **textureSol;
    int nbTextureSol;
    SDL_Texture **textureMur;
    int nbTextureMur;
};
typedef struct texture_map texture_map;

#endif