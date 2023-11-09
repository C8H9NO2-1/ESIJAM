/*
    Fonctions de chargement et de sauvegarde des parametres
*/
#ifndef __PARAMETRE_H__
#define __PARAMETRE_H__

//Format 16:9 des fenetres
#define LARGEUR 16
#define HAUTEUR 9

//Demarrage sur le menu principal
#define START fenetre_menu_principal;

struct parametre{
    int coefResolution;
    SDL_WindowFlags typeEcran;
    int FPS;
};
typedef struct parametre parametre;

enum fenetre{
    fenetre_sortir, fenetre_menu_principal, fenetre_parametre, fenetre_jeu
};
typedef enum fenetre fenetre;

//============================================================================Listes des fonctions

/*Les parametres sont enregistres dans le fichier data/jeu.config
Les fichiers contient les donnees ligne par ligne dans l'ordre suivant :
    -Coeficient de resolution
    -Type d'affichage (0 : fenetre; 1 : plein ecran; 2 plein ecran fenetre)
    -FPS
*/
parametre recupererParametre();


#endif