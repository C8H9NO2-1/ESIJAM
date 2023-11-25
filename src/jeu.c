#include "header/base.h"

#include "header/texture_map.h"
#include "header/camera.h"
#include "header/map.h"
#include "header/parametre.h"
#include "header/entites.h"
#include "header/texture_entites.h"
#include "header/graphe.h"
#include "header/ui.h"
#include "header/finThread.h"
 
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
    int *nbEnnemiRestant;
    bool *exist;

    int *points;
};
typedef struct argUniteEnnemie argUniteEnnemie;

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
    int *nbEnnemiR = arg->nbEnnemiRestant;
    int *points = arg->points;

    while (*running && !(*defeat) && *exist) {
        Uint64 frame_start = SDL_GetTicks64();

        uniteEnnemie(entite, listeEntite, m, defeat, exist);

        while(SDL_GetTicks64() - frame_start < 700)
            SDL_Delay(1);
    }

    *points += 3;
    *nbEnnemiR -= 1;

    *fin = true;
    return NULL;
}

void *ami(void *data) {
    argUniteAllie *arg = (argUniteAllie*) data;
    bool *running = arg->running;
    Entite *entite = arg->entite;
    ListeEntite *listeEntite = arg->listeEntite;
    map *m = arg->m;
    bool *defeat = arg->defeat;
    Graphe *graphe = arg->graphe;

    bool *exist = arg->exist;
    // bool exist = true;

    while (*running && !(*defeat) && *exist) {
        Uint64 frame_start = SDL_GetTicks64();

        if (entite->nouvelObjectif) {
            nouveauCheminAmi(entite, listeEntite, m, *graphe, entite->objectif);
            entite->nouvelObjectif = false;
        }

        uniteAmie(entite, listeEntite, m, exist);

        while(SDL_GetTicks64() - frame_start < 700)
            SDL_Delay(1);
    }


    return NULL;
}


struct argPhase{
    int *numeroDeVague;
    float *dureeEntreChaqueVague;//En minute
    int *nombreDEnnemie;
    float *tauxDEnnemisEntreVague;
    bool *periodePause;
    int *nombreDEnnemieRestant;
    bool* running;
    int *PV_poulpy;
    bool *fin;
};
typedef struct argPhase argPhase;

void *phase(void *data){
    argPhase *argu = (argPhase*) data;
    int *numeroDeVague = argu->numeroDeVague;
    float *dureeEntreChaqueVague = argu->dureeEntreChaqueVague;
    int *nombreDEnnemie = argu->nombreDEnnemie;
    float *tauxDEnnemisEntreVague = argu->tauxDEnnemisEntreVague;
    bool *periodePause = argu->periodePause;
    int *nombreDEnnemieRestant = argu->nombreDEnnemieRestant;
    bool *running = argu->running;
    int *PV_poulpy = argu->PV_poulpy;
    bool *fin = argu->fin;

    while(*running){
        if(*periodePause){
            *nombreDEnnemie *= (*tauxDEnnemisEntreVague);
            SDL_Delay((*dureeEntreChaqueVague)*60*1000);
            *nombreDEnnemieRestant = *nombreDEnnemie;
            *periodePause = false; 
            (*numeroDeVague)++;
        }
        else{
            if(((*nombreDEnnemieRestant) <= 0) || (*PV_poulpy < 0)){
                *periodePause = true;
                if(*PV_poulpy <= 0){
                    *running = false;
                }
            }
            SDL_Delay(2);
        }
    }
    *fin = true;
    return NULL;
}

struct argAjoutEnnemi{
    bool *running;
    bool *fin;
    bool *periodePause;
    int *nombreEnnemi;
    int *nombreEnnemiRestant;
    ListeEntite *listeEntite;
    texture_entite *tE;
    bool *defeat;
    map *M;
    listeFin *liste_fin;
    ListeCheminsEnnemis *listeCheminsEnnemis;

    int *points;
};
typedef struct argAjoutEnnemi argAjoutEnnemi;

void *ajoutEnnemi(void *data){
    argAjoutEnnemi *arg = (argAjoutEnnemi*) data;
    bool *running = arg->running;
    bool *fin = arg->fin;
    bool *periodePause = arg->periodePause;
    ListeEntite *listeEntite = arg->listeEntite;
    texture_entite *tE = arg->tE;
    map* M = arg->M;
    int *nombreEnnemie = arg->nombreEnnemi;
    int *nombreEnnemieRestant = arg->nombreEnnemiRestant;
    bool *defeat = arg->defeat;
    listeFin *liste_fin = arg->liste_fin;
    ListeCheminsEnnemis *listeCheminsEnnemis = arg->listeCheminsEnnemis;

    int *points = arg->points;

    int nbUniteAFaireApparaitre = 0;

    int indiceEnnemi = 0;
    bool existEnnemi[100000];

    while(*running){
        Uint64 frame_start = SDL_GetTicks64();
        if(*periodePause){
            nbUniteAFaireApparaitre = *nombreEnnemie;
            SDL_Delay(50);
        }
        else if(nbUniteAFaireApparaitre > 0){
            Entite *entite = malloc(sizeof(Entite));
            bool *finEntite = malloc(sizeof(bool));
            *finEntite = false;
            ajouteListeFin(liste_fin, finEntite);
            existEnnemi[indiceEnnemi] = true;
            argUniteEnnemie arguments = {running, entite, listeEntite, M, finEntite, defeat, nombreEnnemieRestant, &existEnnemi[indiceEnnemi], points};
            indiceEnnemi++;
            pthread_t threadEnnemi;
            switch(frame_start%4){
                case 0 :
                    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {M->largeur / 4, 0}, listeEntite, tE, false);
                    entite->element = listeCheminsEnnemis->chemin1->premier;
                    pthread_create(&threadEnnemi, NULL, ennemi, &arguments);
                    break;
                case 1:
                    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {24, M->hauteur / 4}, listeEntite, tE, false);
                    entite->element = listeCheminsEnnemis->chemin2->premier;
                    pthread_create(&threadEnnemi, NULL, ennemi, &arguments);
                    break;
                case 2:
                    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {M->largeur / 4, 24}, listeEntite, tE, false);
                    entite->element = listeCheminsEnnemis->chemin3->premier;
                    pthread_create(&threadEnnemi, NULL, ennemi, &arguments);
                    break;
                case 3:
                    initialiserEntite(entite, ENNEMI, UNITE, (Coordonnees) {0, M->hauteur / 4}, listeEntite, tE, false);
                    entite->element = listeCheminsEnnemis->chemin4->premier;
                    pthread_create(&threadEnnemi, NULL, ennemi, &arguments);
                    break;

            }
            nbUniteAFaireApparaitre--;
            SDL_Delay(1500);
        }
        else{
            SDL_Delay(5);
        }
    }
    *fin = true;
    return NULL;
}

int jeu(SDL_Window *window, parametre *para){
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool running = true;
    listeFin liste_fin = initListeFin();
    texture_map* tM;
    tM = chargeTextureMap("data/texture/murSolPlafond.png", renderer);

    map *M;
    M = lecturePseudoMap("data/map/pseudoMap1.pm", tM, 586);
    bool defeat = false;

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
    float dureeEntreChaqueVague = 35./60.;//En minute
    // float dureeEntreChaqueVague = 60./60.;//En minute
    // float dureeEntreChaqueVague = 1./60.;//En minute
    int nombreDEnnemie = 5;
    float tauxDEnnemisEntreVague = 1.2;
    bool periodePause = true;

    int nombreDEnnemieRestant = 5;

    //!========================== Fin système de Vague ================
    //----------------------------------------Teste UI----------------
    ui_liste *l = initList_ui();
    TTF_Font *font = TTF_OpenFont("data/fonts/roboto.ttf", 2000);

    TTF_CloseFont(font);
    //---------------------------------------Fin Teste UI--------------

    //!========== Pour les unités alliées et les pièges ==========
    int indiceUniteAllie = 0;
    argUniteAllie argumentsAllies[1000];
    Entite *unitesAllie[1000];
    pthread_t threadAllie[1000];
    bool existAllie[1000];

    int indicePiege = 0;
    argUniteAllie argumentsPiege[1000];
    Entite *pieges[1000];
    pthread_t threadPiege[1000];
    bool existPiege[1000];

    //!========== Fin des unités alliées et les pièges ==========

    //!========== Code de Test ==========

    Entite *nexus = malloc(sizeof(Entite));

    ListeEntite *listeEntite = initialiserListeEntite(*M);
    texture_entite *tE_ennemi;
    chargerTextureEntite(&tE_ennemi, "data/texture/sprite2.png", "data/texture/sprite2.png", renderer);

    texture_entite *tE_allie;
    chargerTextureEntite(&tE_allie, "data/texture/sprite.png", "data/texture/sprite3.png", renderer);

    texture_entite *textureNexus;
    chargerTextureNexus(&textureNexus, "data/texture/betaNexus.png", renderer);

    texture_entite *texturePiege1;
    chargerTexturePiege1(&texturePiege1, "data/texture/piege1.png", renderer);

    texture_entite *texturePiege2;
    chargerTexturePiege2(&texturePiege2, "data/texture/piege2.png", renderer);

    initialiserEntite(nexus, AMI, UNITE, (Coordonnees) {M->largeur / 4, M->hauteur / 4}, listeEntite, textureNexus, true);
    bool existNexus = true;
    argUniteAllie argumentsNexus = {&running, nexus, listeEntite, M, &defeat, NULL, &existNexus};
    pthread_t threadNexus;
    pthread_create(&threadNexus, NULL, ami, &argumentsNexus);

    // On met les points de vie du nexus très haut pour les tests
    // listeEntite->entites[M->largeur / 4][M->hauteur / 4][0]->pointsVie = 1000000;

    Graphe graphe = matriceAdjacences(*M, listeEntite);

    ListeCheminsEnnemis *listeCheminsEnnemis = calculeCheminsEnnemis(graphe, *M);

    // printf("Test\n");

    // Entite *test = malloc(sizeof(Entite));
    // initialiserEntite(test, AMI, UNITE, (Coordonnees) {M->largeur / 4 + 3, M->hauteur / 4 - 1}, listeEntite, tE_allie, false);
    // bool *finTest = false;
    // bool exist1 = true;

    // argUniteAllie argsAllies = {&running, test, listeEntite, M, finTest, &defeat, &graphe, &exist1};
    // pthread_t threadAllie1;
    // pthread_create(&threadAllie1, NULL, ami, &argsAllies);

    // printf("Test\n");

    // Entite *test2 = malloc(sizeof(Entite));
    // initialiserEntite(test2, ENNEMI, UNITE, (Coordonnees) {M->largeur / 4, 0}, listeEntite, tE_ennemi, false);
    // bool *finTest2 = false;
    // bool exist2 = true;
    
    // test2->element = listeCheminsEnnemis->chemin1->premier;

    // argUniteEnnemie argsEnnemieTest = {&running, test2, listeEntite, M, finTest2, &defeat, &nombreDEnnemieRestant, &exist2};
    // pthread_t threadEnnemi1;
    // pthread_create(&threadEnnemi1, NULL, ennemi, &argsEnnemieTest);

    // printf("Test\n");

    //!========== Fin du Test ==========
    float zoomMin = zoomMinDetermination(M, para);

    //Execution du thread pour les phases
    argPhase arg_phase;
    arg_phase.fin = malloc(sizeof(bool));
    *(arg_phase.fin) = false;
    ajouteListeFin(&liste_fin, arg_phase.fin);
    arg_phase.dureeEntreChaqueVague = &dureeEntreChaqueVague;
    arg_phase.nombreDEnnemie = &nombreDEnnemie;
    arg_phase.nombreDEnnemieRestant = &nombreDEnnemieRestant;
    arg_phase.numeroDeVague = &numeroDeVague;
    arg_phase.periodePause = &periodePause;
    arg_phase.PV_poulpy = &(listeEntite->entites[M->largeur / 4][M->hauteur / 4][0]->pointsVie);
    arg_phase.running = &running;
    arg_phase.tauxDEnnemisEntreVague = &tauxDEnnemisEntreVague;
    pthread_t threadPhase;
    pthread_create(&threadPhase, NULL, phase, &arg_phase);
    //Execution du thread pour l'ajout d'ennemi
    argAjoutEnnemi arg_ajoutEnnemi;
    arg_ajoutEnnemi.fin = malloc(sizeof(bool));
    *(arg_ajoutEnnemi.fin) = false;
    ajouteListeFin(&liste_fin, arg_ajoutEnnemi.fin);
    arg_ajoutEnnemi.defeat = &defeat;
    arg_ajoutEnnemi.liste_fin = &liste_fin;
    arg_ajoutEnnemi.listeCheminsEnnemis = listeCheminsEnnemis;
    arg_ajoutEnnemi.listeEntite = listeEntite;
    arg_ajoutEnnemi.M = M;
    arg_ajoutEnnemi.nombreEnnemi = &nombreDEnnemie;
    arg_ajoutEnnemi.nombreEnnemiRestant = &nombreDEnnemieRestant;
    arg_ajoutEnnemi.periodePause = &periodePause;
    arg_ajoutEnnemi.running = &running;
    arg_ajoutEnnemi.tE = tE_ennemi;
    arg_ajoutEnnemi.points = &point;
    pthread_t threadAjoutEnnemi;
    pthread_create(&threadAjoutEnnemi, NULL, ajoutEnnemi, &arg_ajoutEnnemi);
    //Execution du thread pour la video
    bool *finAffichageVideo = malloc(sizeof(bool));
    ajouteListeFin(&liste_fin, finAffichageVideo);
    camera cam = initCamera((float) LARGEUR*para->coefResolution/2, HAUTEUR*para->coefResolution/2, zoomMin, LARGEUR*para->coefResolution, HAUTEUR*para->coefResolution);
    argAfficheVideo arg = {&running, para, &cam, renderer, tM, M, finAffichageVideo, l, listeEntite};
    pthread_t threadVideo;
    pthread_create(&threadVideo, NULL, afficheVideo, &arg);
  

    //!========== Attention ==========
    // periodePause = false;
    // !========== Fin Attention ======

    SDL_Event e;
    SDL_Event e2;
    bool editionPiege = false;
    while(running){
        if (defeat) {

            SDL_Delay(10000);
            running = false;
        }
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
                        case SDLK_p:
                            editionPiege = true;
                            while (editionPiege && periodePause) {
                                SDL_PollEvent(&e2);
                                switch (e2.type){
                                    case SDL_KEYDOWN:
                                        switch (e2.key.keysym.sym){
                                            case SDLK_p:
                                                editionPiege = false;
                                                break;
                                            default:
                                                break;
                                        }
                                        break;
                                    case SDL_MOUSEBUTTONDOWN:
                                        switch (e2.button.button) {
                                            case SDL_BUTTON_LEFT:
                                                if (point >= coutPiege) {
                                                    printf("Nouveau piege\n");
                                                    if (posePiege1(listeEntite, M, pieges, indicePiege, texturePiege1, (Coordonnees) {e2.button.x, e2.button.y}, &cam)) {
                                                        point -= coutPiege;
                                                        existPiege[indicePiege] = true;
                                                        argumentsPiege[indicePiege] = (argUniteAllie) {&running, pieges[indicePiege], listeEntite, M, &defeat, &graphe, &existPiege[indicePiege]};
                                                        pthread_create(&threadPiege[indicePiege], NULL, ami, &argumentsPiege[indicePiege]);
                                                        indicePiege++;
                                                    }
                                                }
                                                break;
                                            case SDL_BUTTON_RIGHT:
                                                if (point >= coutPiege) {
                                                    printf("Nouveau piege\n");
                                                    if (posePiege2(listeEntite, M, pieges, indicePiege, texturePiege2, (Coordonnees) {e2.button.x, e2.button.y}, &cam)) {
                                                        point -= coutPiege;
                                                        existPiege[indicePiege] = true;
                                                        argumentsPiege[indicePiege] = (argUniteAllie) {&running, pieges[indicePiege], listeEntite, M, &defeat, &graphe, &existPiege[indicePiege]};
                                                        pthread_create(&threadPiege[indicePiege], NULL, ami, &argumentsPiege[indicePiege]);
                                                        indicePiege++;
                                                    }
                                                }
                                                break;
                                            default:
                                                break;
                                        }
                                        break;
                                    default:
                                        break;
                                }
                            }

                        default:
                            controlCam(&cam, 40, 0.05, &e, 1, zoomMin);
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            printf("Nouvelle entite\n");
                            selectionneEntite(listeEntite, (Coordonnees) {e.button.x, e.button.y}, M, &cam);
                            break;
                        case SDL_BUTTON_RIGHT:
                            donnerObjectif(listeEntite, (Coordonnees) {e.button.x, e.button.y}, M, &cam);
                            break;
                        default:
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
                        case SDLK_a:
                            if (point >= coutAllie) {
                                point -= coutAllie;  
                                printf("Nouvelle entite\n");
                                if (spawnAllie(listeEntite, M, unitesAllie, indiceUniteAllie, tE_allie)) {
                                    existAllie[indiceUniteAllie] = true;
                                    argumentsAllies[indiceUniteAllie] = (argUniteAllie) {&running, unitesAllie[indiceUniteAllie], listeEntite, M, &defeat, &graphe, &existAllie[indiceUniteAllie]};
                                    pthread_create(&threadAllie[indiceUniteAllie], NULL, ami, &argumentsAllies[indiceUniteAllie]);
                                    indiceUniteAllie++;
                                }
                            }
                            break;
                        default:
                            controlCam(&cam, 40, 0.05, &e, 1, zoomMin);
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            selectionneEntite(listeEntite, (Coordonnees) {e.button.x, e.button.y}, M, &cam);
                            break;
                        case SDL_BUTTON_RIGHT:
                            donnerObjectif(listeEntite, (Coordonnees) {e.button.x, e.button.y}, M, &cam);
                            break;
                        default:
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
    // supprimeListeFin(liste_fin);
    

    freeListe_ui(l);
    freeListeCheminsEnnemis(listeCheminsEnnemis);
    freeGraphe(graphe, *M);
    freeMap(M);
    freeTextureMap(tM);
    SDL_DestroyRenderer(renderer);
    return fenetre_parametre;
}