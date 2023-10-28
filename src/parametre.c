#include "header/base.h"

parametre recupererParametre(){
    /*Les parametres sont enregistres dans le fichier data/jeu.config
    Les fichiers contient les donnees ligne par ligne dans l'ordre suivant :
    Coeficient de resolution
    Type d'affichage (0 : fenetre; 1 : plein ecran; 2 plein ecran fenetre)
    FPS
    */
    parametre i;
    int typeEcran;

    FILE* fichier = fopen(PARAMETER_FILE, "r");
    if(fichier == NULL) exit(1);
    fscanf(fichier, "%d\n", &i.coefResolution);
    fscanf(fichier, "%d\n", &typeEcran);
    switch (typeEcran)
    {
    case 0:
        i.typeEcran = 0;
        break;
    case 1:
        i.typeEcran = SDL_WINDOW_FULLSCREEN;
        break;
    case 2:
        i.typeEcran = SDL_WINDOW_FULLSCREEN_DESKTOP;
    default:
        exit(2);
        break;
    }
    fscanf(fichier, "%d", &i.FPS);
    fclose(fichier);

    return i;
}