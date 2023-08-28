#include "game.h"
#include "initialize.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initilize(g)) {
        return true;
    }

    SDL_Color outer_color = {200, 100, 150, 255};
    SDL_Color center_color = {50, 50, 150, 255};

    if (bubble_text_new(&g->bubble_text, g->renderer, "Bubble Text", 100, 20, outer_color, center_color)) {
        return true;
    }

    if (fps_new(&g->fps)) {
        return true;
    }

    g->playing = true;

    return false;
}

void game_free(struct Game *g) {
    fps_free(g->fps);
    bubble_text_free(g->bubble_text);
    SDL_DestroyRenderer(g->renderer);
    SDL_DestroyWindow(g->window);
    TTF_Quit();
    SDL_Quit();
    free(g);
}

bool game_run(struct Game *g) {

    while (g->playing) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
                case SDL_QUIT:
                    return false;
                    break;
                case SDL_KEYDOWN:
                    switch (g->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            return false;
                            break;
                        case SDL_SCANCODE_F:
                            fps_toggle_display(g->fps);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        bubble_text_update(g->bubble_text, g->delta_time);

        if (SDL_RenderClear(g->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return true;
        }

        if (bubble_text_draw(g->bubble_text)) {
            return true;
        }
        
        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }
    
    return false;
}