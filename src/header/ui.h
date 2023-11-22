#ifndef __UI_H__
#define __UI_H__

enum ui_type{
    UI_LABEL, UI_BOUTON, UI_SELECT, UI_ONGLET
};
typedef enum ui_type ui_type;

typedef struct ui_maille ui_maille;
struct ui_maille{
    ui_type t;
    void* objet;
    ui_maille *next;

};
typedef ui_maille* ui_liste;

struct bouton_ui
{
    int x;
    int y;
    int w;
    int h;
    const char *text;
    void (*action)(void*);
    void *data;
    SDL_Texture *t_nothing;
    SDL_Texture *t_on;
    SDL_Texture *t_click;
    SDL_Texture *t_afficher;
};
typedef struct bouton_ui bouton_ui;

struct label_ui{
    int x;
    int y;
    int w;
    int h;
    const char *text;
    SDL_Texture *t;
};
typedef struct label_ui label_ui;


ui_liste* initList_ui();
void ajoutListe_ui(ui_liste *l, ui_type t, void *objet);
void afficherListe_ui(ui_liste *l, SDL_Renderer *renderer);
void eventListe_ui(ui_liste *l, SDL_Event *e);
void freeListe_ui(ui_liste *l);
void initBouton_ui(ui_liste *l, int x, int y, int w, int h, const char* text, void (*action)(void*), void *data, SDL_Renderer *renderer, TTF_Font *f);
void afficheBouton_ui(SDL_Renderer *renderer, bouton_ui *b);
void freeBouton_ui(bouton_ui *b);
void initLabel_ui(ui_liste *l, int x, int y, int w, int h, const char* text, SDL_Renderer *renderer, TTF_Font *f);


#endif