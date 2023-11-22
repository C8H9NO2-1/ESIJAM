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
    ui_maille *previous;

};
typedef ui_maille* ui_liste;

struct bouton_ui
{
    int x;
    int y;
    int w;
    int h;
    char *text;
    void (*action)(void*);
    void *data;
    SDL_Texture *t_nothing;
    SDL_Texture *t_on;
    SDL_Texture *t_click;
};
typedef struct bouton_ui bouton_ui;


#endif