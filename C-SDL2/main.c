// Included header files.
#include "main.h"
#include "sdl_init.h"
#include "game.h"

int main() {
    struct Sdl *sdl = sdl_new();
    if (!sdl) {
        return 1;
    } else if (sdl->error) {
        sdl_free(sdl);
        return 1;
    }

    struct Game *game = game_new(sdl->renderer);
    if (!game) {
        sdl_free(sdl);
        return 1;
    }

    int exit_val = 1;
    if (game_run(game)) {
        exit_val = 0;
    }
    
    game_free(game);
    sdl_free(sdl);
    return exit_val;
}