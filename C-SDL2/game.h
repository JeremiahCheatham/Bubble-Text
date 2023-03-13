#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "bubble_text.h"

struct Game {
    SDL_Event event;
    SDL_Renderer *renderer;
    float frame_delay;
    bool show_fps;
    float delta_time;
    struct Bubble_Text *bubble_text;
};

struct Game* game_new(SDL_Renderer *renderer);
bool game_run(struct Game *this);
void game_free(struct Game *this);

#endif // GAME_H