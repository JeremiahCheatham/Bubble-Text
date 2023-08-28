#ifndef BUBBLE_TEXT_H
#define BUBBLE_TEXT_H

#include "main.h"

struct Bubble_Text {
    SDL_Renderer *renderer;
    TTF_Font *font;
    SDL_Surface *text_surf;
    SDL_Surface *target_surf;
    SDL_Texture* image;
    SDL_Rect rect;
    double x_pos;
    double y_pos;
    double speed;
    double x_vel;
    double y_vel;
};

bool bubble_text_new(
    struct Bubble_Text **bubble_text,
    SDL_Renderer *renderer,
    const char* text,
    int text_size,
    int radius,
    SDL_Color outer_color,
    SDL_Color center_color
);
void bubble_text_free(struct Bubble_Text *g);
void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf, int radius, int x, int y);
void bubble_text_update(struct Bubble_Text *g, double dt);
bool bubble_text_draw(struct Bubble_Text *g);

#endif