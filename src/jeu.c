#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"
 

void teste1(void *data){
    printf("%s\n", (char*) data);
}

//Argument pour le second thread
struct argAfficheVideo{
    bool *running;
    parametre *para;
    camera *cam;
    SDL_Renderer *renderer;
    texture_map *tM;
    map *M;
    bool *fin;
    ui_liste *l;
    ListeEntite *listeEntite;
};
typedef struct argAfficheVideo argAfficheVideo;

struct argUniteEnnemie {
    bool *running;
    Entite *entite;
    ListeEntite *listeEntite;
    map *m;
    bool *fin;
    bool *defeat;
    bool *exist;
};
typedef struct argUniteEnnemie argUniteEnnemie;

struct argUniteAllie {
    bool *running;
    Entite *entite;
    ListeEntite *listeEntite;
    map *m;
    bool *fin;
    bool *defeat;
    bool *exist;
};
typedef struct argUniteAllie argUniteAllie;
//Fonction qui est executer dans un autre thread que le principal et qui permet d'afficher selon des FPS
void *afficheVideo(void *data){
    argAfficheVideo *arg = (argAfficheVideo*) data;
    bool *running = arg->running;
    camera *cam = arg->cam;
    map *M = arg->M;
    texture_map *tM = arg->tM;
    parametre *para = arg->para;
    SDL_Renderer *renderer = arg->renderer;
    bool* fin = arg->fin;
    ui_liste *l = arg->l;

    ListeEntite *listeEntite = arg->listeEntite;

    while(*running) {
        Uint64 frame_start = SDL_GetTicks64();

        SDL_RenderClear(renderer);
        //Fond noir
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x00);

        SDL_RenderFillRect(renderer, NULL);
        printf("OK !\n");
        afficheMapCamera(cam, M, renderer, tM, listeEntite);
        printf("OK 1\n");
        afficherListe_ui(l, renderer);
        printf("OK 2\n"); 
        SDL_RenderPresent(renderer);
        printf("OK 3\n");

        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    freeListe_ui(l);
    *fin = true;
    return NULL;
}

void *unite(void *data) {
    argUniteEnnemie *arg = (argUniteEnnemie*) data;
    bool *running = arg->running;
    Entite *entite = arg->entite;
    ListeEntite *listeEntite = arg->listeEntite;
    map *m = arg->m;
    bool *fin = arg->fin;
    bool *defeat = arg->defeat;
    bool *exist = arg->exist;

    while (*running && !(*defeat) && *exist) {
        Uint64 frame_start = SDL_GetTicks64();

        uniteEnnemie(entite, listeEntite, m, defeat, exist);

        while(SDL_GetTicks64() - frame_start < 1000 / 5)
            SDL_Delay(1);
    }

    *fin = true;
    return NULL;
}

void *uniteAllie(void *data) {
    argUniteAllie *arg = (argUniteAllie*) data;
    bool *running = arg->running;
    Entite *entite = arg->entite;
    ListeEntite *listeEntite = arg->listeEntite;
    map *m = arg->m;
    bool *fin = arg->fin;
    bool *defeat = arg->defeat;
    bool *exist = arg->exist;

    while (*running && !(*defeat) && *exist) {
        Uint64 frame_start = SDL_GetTicks64();

        uniteAmie(entite, listeEntite, m, exist);

        while(SDL_GetTicks64() - frame_start < 1000 / 5)
            SDL_Delay(1);
    }

    *fin = true;
    return NULL;
}

int jeu(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    bool fin = false;
    texture_map* tM;
    tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

    map *M;
    M = lecturePseudoMap("data/map/pseudoMap1.pm", tM, 586);

    
    //----------------------------------------Teste UI----------------
    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);
    initBouton_ui(l, 100, 100, 100, 50, "Bonjour", teste1, "Bonjour", renderer, font);
    initBouton_ui(l, 1000, 100, 100, 50, "Hello Wolrd", teste1, "Crève", renderer, font);

    TTF_CloseFont(font);
    //---------------------------------------Fin Teste UI--------------

    //!========== Code de Test ==========

    Entite *entite = malloc(sizeof(Entite));
    Entite *entiteEnnemie2 = malloc(sizeof(Entite));
    Entite *allie = malloc(sizeof(Entite));
    ListeEntite *listeEntite = initialiserListeEntite(*M);
    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {M->largeur / 4, 0}, listeEntite);
    initialiserEntite(entiteEnnemie2, ENNEMI, UNITE, (Coordonnees) {M->largeur / 2 - 1, M->hauteur / 4}, listeEntite);
    initialiserEntite(allie, AMI, UNITE, (Coordonnees) {M->largeur / 4 + 3, M->hauteur / 4 - 1}, listeEntite);

    chargerTextureEntite(entite->texture, "data/adventurer-Sheet.png", "data/adventurer-Sheet.png", renderer);
    chargerTextureEntite(allie->texture, "data/adventurer-Sheet.png", "data/adventurer-Sheet.png", renderer);
    chargerTextureEntite(entiteEnnemie2->texture, "data/adventurer-Sheet.png", "data/adventurer-Sheet.png", renderer);

    Graphe graphe = matriceAdjacences(*M, listeEntite);

    // int nbreLigne = (M->hauteur * M->largeur) / 4;

    // printf("%d\n", nbreLigne);
    // SDL_Delay(1000);

    // // for (int i = 0; i < nbreLigne; i++) {
    // //     for (int j = 0; j < nbreLigne; j++) {
    // //         printf("%d ", graphe.matriceAdjacenceEnemi1[i][j]);
    // //     }
    // //     printf("\n");
    // // }

    // printf("Fin de l'affichage de la matrice\n");

    // Coordonnees depart = {M->largeur / 4, 0};
    // Coordonnees arrivee = {M->largeur / 4, M->largeur / 4};

    // printf("Départ: x = %d / y = %d\n", depart.x, depart.y);
    // printf("Arrivée: x = %d / y = %d\n", arrivee.x, arrivee.y);

    ListeCheminsEnnemis *listeCheminsEnnemis = calculeCheminsEnnemis(graphe, *M);
    entite->element = listeCheminsEnnemis->chemin1->premier;
    entiteEnnemie2->element = listeCheminsEnnemis->chemin2->premier;


    // printf("Premier chemin: \n");
    // ElementCheminEnnemi *element = listeCheminsEnnemis->chemin1->premier;
    // while (element->caseSuivante != NULL) {
    //     printf("x : %d / y : %d\n", element->coordonnees.x, element->coordonnees.y);
    //     element = element->caseSuivante;
    // }
    // printf("\n");

    // printf("Deuxième chemin: \n");
    // element = listeCheminsEnnemis->chemin2->premier;
    // while (element->caseSuivante != NULL) {
    //     printf("x : %d / y : %d\n", element->coordonnees.x, element->coordonnees.y);
    //     element = element->caseSuivante;
    // }
    // printf("\n");

    // printf("Troisième chemin: \n");
    // element = listeCheminsEnnemis->chemin3->premier;
    // while (element->caseSuivante != NULL) {
    //     printf("x : %d / y : %d\n", element->coordonnees.x, element->coordonnees.y);
    //     element = element->caseSuivante;
    // }
    // printf("\n");

    // printf("Quatrième chemin: \n");
    // element = listeCheminsEnnemis->chemin4->premier;
    // while (element->caseSuivante != NULL) {
    //     printf("x : %d / y : %d\n", element->coordonnees.x, element->coordonnees.y);
    //     element = element->caseSuivante;
    // }
    // printf("\n");

    //!========== Fin du Test ==========

    bool defeat = false;
    bool finEntite = false;
    bool existEntite1 = true;
    argUniteEnnemie arguments = {&running, entite, listeEntite, M, &finEntite, &defeat, &existEntite1};
    pthread_t threadEnnemi1;
    pthread_create(&threadEnnemi1, NULL, unite, &arguments);

    bool finEntite2 = false;
    bool existEntite2 = true;
    argUniteAllie arguments2 = {&running, allie, listeEntite, M, &finEntite2, &defeat, &existEntite2};
    pthread_t threadAllie1;
    pthread_create(&threadAllie1, NULL, uniteAllie, &arguments2);

    bool finEntite3 = false;
    bool existEntite3 = true;
    argUniteEnnemie argumentsEnnemi2 = {&running, entiteEnnemie2, listeEntite, M, &finEntite3, &defeat, &existEntite3};
    pthread_t threadEnnemi2;
    // pthread_create(&threadEnnemi2, NULL, unite, &argumentsEnnemi2);

    nouveauCheminAmi(allie, listeEntite, M, graphe, (Coordonnees) {M->largeur / 4, 1});
    // printf("Premier chemin: \n");
    // while (allie->element->caseSuivante != NULL) {
    //     printf("x : %d / y : %d\n", allie->element->coordonnees.x, allie->element->coordonnees.y);
    //     allie->element = allie->element->caseSuivante;
    // }

    float zoomMin = zoomMinDetermination(M, para);

    //Execution du second thread pour la video
    camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, zoomMin, LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);
    argAfficheVideo arg = {&running, para, &cam, renderer, tM, M, &fin, l, listeEntite};
    pthread_t threadVideo;
    pthread_create(&threadVideo, NULL, afficheVideo, &arg);
  

    SDL_Event e;
    while(running){
        SDL_WaitEvent(&e);
        switch (e.type){
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym){
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_g:
                        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                        break;  
                    case SDLK_s:
                        SDL_SetWindowFullscreen(window, 0);
                        break;
                    default:
                        controlCam(&cam, 20, 0.05, &e, 1, zoomMin);
                        break;
                }
                break;
        
            default:
                break;
        }
        eventListe_ui(l, &e);

        }
    //Attente de fin d'execution du second thread
    while(!fin) SDL_Delay(50);
    while(!finEntite) SDL_Delay(50);
    while(!finEntite2) SDL_Delay(50);
    

    freeListe_ui(l);
    freeListeCheminsEnnemis(listeCheminsEnnemis);
    freeGraphe(graphe, *M);
    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return 0;
}