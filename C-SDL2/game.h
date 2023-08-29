#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "bubble_text.h"
#include "fps.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct Bubble_Text *bubble_text;
    struct Fps *fps;
    bool playing;
    double delta_time;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
bool game_run(struct Game *g);

#endif