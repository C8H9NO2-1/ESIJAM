#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"
 
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
    bool *nouveauCheminNecessaire;
    Coordonnees *destination;
    Graphe *graphe;
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
        afficheMapCamera(cam, M, renderer, tM, listeEntite);
        afficherListe_ui(l, renderer);
        SDL_RenderPresent(renderer);

        while(SDL_GetTicks64() - frame_start < 1000 / (Uint64)para->FPS)
            SDL_Delay(1 /* ms */);
    }

    freeListe_ui(l);
    *fin = true;
    return NULL;
}

void *ennemi(void *data) {
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

void *ami(void *data) {
    argUniteAllie *arg = (argUniteAllie*) data;
    bool *running = arg->running;
    Entite *entite = arg->entite;
    ListeEntite *listeEntite = arg->listeEntite;
    map *m = arg->m;
    bool *fin = arg->fin;
    bool *defeat = arg->defeat;
    bool *exist = arg->exist;
    bool *nouveauCheminNecessaire = arg->nouveauCheminNecessaire;
    Coordonnees *destination = arg->destination;
    Graphe *graphe = arg->graphe;

    while (*running && !(*defeat) && *exist) {
        Uint64 frame_start = SDL_GetTicks64();

        if (*nouveauCheminNecessaire) {
            nouveauCheminAmi(entite, listeEntite, m, *graphe, *destination);
            *nouveauCheminNecessaire = false;
        }

        uniteAmie(entite, listeEntite, m, exist);

        while(SDL_GetTicks64() - frame_start < 1000 / 5)
            SDL_Delay(1);
    }

    *fin = true;
    return NULL;
}


struct argPhase{
    int numeroDeVague;
    float dureeEntreChaqueVague;//En minute
    int nombreDEnnemie;
    float tauxDEnnemisEntreVague;
    bool periodePause;
    int nombreDEnnemieRestant;
};
typedef struct argPhase argPhase;

void phase(void *data){
    argPhase *argu = data;
}

int jeu(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    bool fin = false;
    texture_map* tM;
    tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

    map *M;
    M = lecturePseudoMap("data/map/pseudoMap1.pm", tM, 586);

    //!========================== Dégats ============================
    int degatUnite = 4;
    int degatPiege = 7;
    //!========================== Fin Dégats ========================
    //!========================== Système de monnaie ================
    int point = 10;
    int coutPiege = 2;
    int coutAllie = 4;
    int PointGagne = 3;
    //!========================== Fin système de monnaie ============
    //!=========================== Système de Vague =================
    int numeroDeVague =1;
    float dureeEntreChaqueVague = 2.;//En minute
    int nombreDEnnemie = 5;
    float tauxDEnnemisEntreVague = 1.2;
    bool periodePause = true;

    int nombreDEnnemieRestant = 0;

    //!========================== Fin système de Vague ================
    //----------------------------------------Teste UI----------------
    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);

    TTF_CloseFont(font);
    //---------------------------------------Fin Teste UI--------------

    //!========== Code de Test ==========

    Entite *nexus = malloc(sizeof(Entite));

    Entite *entite = malloc(sizeof(Entite));
    Entite *entite2 = malloc(sizeof(Entite));
    Entite *entite3 = malloc(sizeof(Entite));
    ListeEntite *listeEntite = initialiserListeEntite(*M);
    texture_entite *tE;
    chargerTextureEntite(&tE, "data/texture/sprite.png", renderer);

    texture_entite *tE2;
    chargerTextureEntite(&tE2, "data/texture/sprite2.png", renderer);

    texture_entite *textureNexus;
    chargerTextureEntite(&textureNexus, "data/texture/betaNexus.png", renderer);

    initialiserEntite(nexus, AMI, UNITE, (Coordonnees) {M->largeur / 4, M->hauteur / 4}, listeEntite, textureNexus);

    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {M->largeur / 4, 0}, listeEntite, tE2);
    initialiserEntite(entite2, ENNEMI, UNITE, (Coordonnees) {M->largeur / 2 - 1, M->hauteur / 4}, listeEntite, tE2);
    initialiserEntite(entite3, AMI, UNITE, (Coordonnees) {M->largeur / 4 + 3, M->hauteur / 4}, listeEntite, tE);

    // On met les points de vie du nexus très haut pour les tests
    listeEntite->entites[M->largeur / 4][M->hauteur / 4][0]->pointsVie = 1000000;

    Graphe graphe = matriceAdjacences(*M, listeEntite);

    ListeCheminsEnnemis *listeCheminsEnnemis = calculeCheminsEnnemis(graphe, *M);
    entite->element = listeCheminsEnnemis->chemin1->premier;
    entite2->element = listeCheminsEnnemis->chemin2->premier;

    //!========== Fin du Test ==========

    bool defeat = false;
    bool finEntite = false;
    bool existEntite1 = true;
    argUniteEnnemie arguments = {&running, entite, listeEntite, M, &finEntite, &defeat, &existEntite1};
    pthread_t threadEnnemi1;
    pthread_create(&threadEnnemi1, NULL, ennemi, &arguments);


    bool finEntite2 = false;
    bool existEntite2 = true;
    argUniteEnnemie arguments2 = {&running, entite2, listeEntite, M, &finEntite2, &defeat, &existEntite2};
    pthread_t threadEnnemi2;
    pthread_create(&threadEnnemi2, NULL, ennemi, &arguments2);

    bool finEntite3 = false;
    bool existEntite3 = true;
    bool nouveauCheminNecessaire = true;
    Coordonnees destination = {M->largeur / 4, 1};
    argUniteAllie arguments3 = {&running, entite3, listeEntite, M, &finEntite3, &defeat, &existEntite3, &nouveauCheminNecessaire, &destination, &graphe};
    pthread_t threadAllie1;
    pthread_create(&threadAllie1, NULL, ami, &arguments3);

    float zoomMin = zoomMinDetermination(M, para);
    //Execution du second thread pour la video
    camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, zoomMin, LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);
    argAfficheVideo arg = {&running, para, &cam, renderer, tM, M, &fin, l, listeEntite};
    pthread_t threadVideo;
    pthread_create(&threadVideo, NULL, afficheVideo, &arg);
  

    SDL_Event e;
    while(running){
        if(periodePause){
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
                        default:
                            controlCam(&cam, 40, 0.05, &e, 1, zoomMin);
                            break;
                    }
                    break;
            
            default:
            break;
            }
            eventListe_ui(l, &e);
        }
        else{
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
                        // case SDLK_a:
                        // printf("Nouvelle entite\n");
                        //     initialiserEntite(entite2, ENNEMI, UNITE, (Coordonnees) {0, M->hauteur/4}, listeEntite, tE);
                        //     entite2->element = listeCheminsEnnemis->chemin3->premier;
                        //     pthread_create(&threadEnnemi2, NULL, unite, &arguments2);
                        //     break;
                        default:
                            controlCam(&cam, 40, 0.05, &e, 1, zoomMin);
                            break;
                    }
                    break;
            
            default:
            break;
            }
            eventListe_ui(l, &e);
        }
    }
    //Attente de fin d'execution du second thread
    while(!fin) SDL_Delay(50);
    while(!finEntite) SDL_Delay(50);
    while(!finEntite2) SDL_Delay(50);
    while(!finEntite3) SDL_Delay(50);
    

    freeListe_ui(l);
    freeListeCheminsEnnemis(listeCheminsEnnemis);
    freeGraphe(graphe, *M);
    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return 0;
}