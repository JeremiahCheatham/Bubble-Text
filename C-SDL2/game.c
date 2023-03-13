// Included header files.
#include "game.h"
#include "fps.h"

struct Game* game_new(SDL_Renderer *renderer) {
    struct Game *this = calloc(1, sizeof(struct Game));
    if (!this) {
        fprintf(stderr, "Error in malloc of game!\n");
        return this;
    }

    this->renderer = renderer;
    this->delta_time = 0;
    this->frame_delay = 1000.0f / FPS;
    this->show_fps = SHOW_FPS;

    return this;
}

void game_free(struct Game *this) {
    bubble_text_free(this->bubble_text);
    free(this);
}

bool game_run(struct Game *this) {

    // The Outer and Inter Bubble colors.
    SDL_Color outer_color = {200, 100, 150, 255};
    SDL_Color center_color = {50, 50, 150, 255};

    this->bubble_text = bubble_text_new(this->renderer, "Bubble Text", 100, 10, outer_color, center_color);
    if (!this->bubble_text || this->bubble_text->error) {
        return false;
    }

    while (true) {

        // Check key events, key pressed or released.
        while (SDL_PollEvent(&this->event)) {
            switch (this->event.type) {

                case SDL_QUIT:
                    // handling of close button
                    return true;
                case SDL_KEYDOWN:
                    // keyboard API for key pressed
                    switch (this->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            return true;
                        default:
                            break;
                    }
            }
        }

        bubble_text_update(this->bubble_text);

        // Clear the existing renderer.
        if (SDL_RenderClear(this->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return false;
        }

        // Draw score to the renderer.
        if (!bubble_text_draw(this->bubble_text)) {
            return false;
        }
        
        // Swap the back buffer to the front.
        SDL_RenderPresent(this->renderer);

        // Print FPS to standard output.
        if (this->show_fps)
            fps_print();

        // Calculate delay needed for the FPS.
        this->delta_time = fps_delay(this->frame_delay);
    }
    
    return true;
}