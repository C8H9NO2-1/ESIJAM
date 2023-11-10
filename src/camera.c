#include "header/base.h"

camera initCamera(float x, float y, float zoom, int w, int h){
    camera cam ={x, y, zoom, w, h};
    return cam;
}

void controlCam(camera* cam, float pas, float pasZoom, SDL_Event* e, float dt){
    switch(e->key.keysym.sym){
        case SDLK_UP:
            if(cam->y-(cam->h/2) - pas*dt > 0)
            cam->y -= pas*dt;
            break;
        case SDLK_DOWN:
            cam->y += pas*dt;
            break;
        case SDLK_LEFT:
            if(cam->x-(cam->w/2) - pas*dt > 0)
            cam->x -= pas*dt;
            break;
        case SDLK_RIGHT:
            cam->x += pas*dt;
            break;
        case SDLK_KP_PLUS :
            if(cam->zoom + pasZoom*dt <= ZOOM_MAX){
                cam->x += (cam->w/4)*dt;//Calcul a refaire car pas terrible
                cam->y += (cam->h/4)*dt;//Calcul a refaire car pas terrible
                cam->zoom += pasZoom*dt;
            }
            break;
        case SDLK_2:
            if(cam->zoom + pasZoom*dt <= ZOOM_MAX){
                cam->x += (cam->w/4)*dt;//Calcul a refaire car pas terrible
                cam->y += (cam->h/4)*dt;//Calcul a refaire car pas terrible
                cam->zoom += pasZoom*dt;
            }
            break;
        case SDLK_KP_MINUS :
            if(cam->zoom - pasZoom*dt >= ZOOM_MIN)
            cam->zoom -= pasZoom*dt;
            break;
        case SDLK_1 :
            if(cam->zoom - pasZoom*dt >= ZOOM_MIN)
            cam->zoom -= pasZoom*dt;
            break;
        default:
            break;
    }

}