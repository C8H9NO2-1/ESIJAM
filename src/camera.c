#include "header/base.h"

#include "header/camera.h"

camera initCamera(float x, float y, float zoom, int w, int h){
    camera cam ={x, y, zoom, w, h};
    return cam;
}

void controlCam(camera* cam, float pas, float pasZoom, SDL_Event* e, float dt, float zoomMin){
    switch(e->key.keysym.sym){
        case SDLK_UP:
            if(cam->y-(cam->h/2) - pas*dt > 0){
                for(int i = 0; i < 30; i++){
                    cam->y -= pas*dt/30;
                    SDL_Delay(3);
                }
            }
            break;
        case SDLK_DOWN:
            for(int i = 0; i < 30; i++){
                cam->y += pas*dt/30;
                SDL_Delay(3);
            }
            break;
        case SDLK_LEFT:
            if(cam->x-(cam->w/2) - pas*dt > 0){
                for(int i = 0; i < 30; i++){
                    cam->x -= pas*dt/30;
                    SDL_Delay(3);
                }
            }
            break;
        case SDLK_RIGHT:
            for(int i = 0; i < 30; i++){
                cam->x += pas*dt/30;
                SDL_Delay(3);
            }
            break;
        case SDLK_KP_PLUS :
            if(cam->zoom + pasZoom*dt/2 <= ZOOM_MAX){
                //cam->x += (cam->w/4)*dt;//Calcul a refaire car pas terrible
                //cam->y += (cam->h/4)*dt;//Calcul a refaire car pas terrible
                for(int i = 0; i < 10; i++){
                    cam->zoom += pasZoom*dt/10;
                    SDL_Delay(5);
                }
            }
            break;
        case SDLK_2:
            if(cam->zoom + pasZoom*dt/2 <= ZOOM_MAX){
                for(int i = 0; i < 10; i++){
                    cam->zoom += pasZoom*dt/10;
                    SDL_Delay(5);
                }
            }
            break;
        case SDLK_KP_MINUS :
            if(cam->zoom - pasZoom*dt/2 >= zoomMin){
                for(int i = 0; i < 10; i++){
                    cam->zoom -= pasZoom*dt/10;
                    SDL_Delay(5);
                }
            }
            break;
        case SDLK_1 :
            if(cam->zoom - pasZoom*dt/2 >= zoomMin)
                for(int i = 0; i < 10; i++){
                    cam->zoom -= pasZoom*dt/10;
                    SDL_Delay(5);
                }
            break;
        default:
            break;
    }

}