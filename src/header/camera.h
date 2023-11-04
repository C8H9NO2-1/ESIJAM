#ifndef __CAMERA_H__
#define __CAMERA_H__

#define ZOOM_MAX 10
#define ZOOM_MIN 0.4

//On considere pour simplifier que w est la largeur de la fenetre et h sa hauteur, aussi les coordonnees x et y sont en pixels
//On considere de plus que la camera est au milieu de l'ecran
struct camera
{
    int x;
    int y;
    float zoom;
    int w;
    int h;
};
typedef struct camera camera;


camera initCamera(float x, float y, float zoom, int w, int h);
void controlCam(camera* cam, float pas, float pasZoom, SDL_Event* e);

#endif