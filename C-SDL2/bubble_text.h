#ifndef BUBBLE_TEXT_H
#define BUBBLE_TEXT_H

#include "main.h"

struct Bubble_Text {
    SDL_Renderer *renderer;
    SDL_Texture* image;
    SDL_Rect rect;
    bool error;
    int vel_x;
    int vel_y;
};

struct Bubble_Text* bubble_text_new(SDL_Renderer *renderer, const char* text, int text_size, float radius, SDL_Color outer_color, SDL_Color center_color);
void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf, int radius, int x, int y);
void bubble_text_update(struct Bubble_Text *this);
bool bubble_text_draw(struct Bubble_Text *this);
void bubble_text_free(struct Bubble_Text *this);

#endif // BUBBLE_TEXT_H