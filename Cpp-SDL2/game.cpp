#include "game.h"
#include "bubble_text.h"
#include "fps.h"

Game::Game() {
    // Initialize SDL2
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        auto error = fmt::format("Error initializing SDL: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    // Create a window
    this->window = SDL_CreateWindow(this->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->width, this->height, SDL_WINDOW_SHOWN);
    if (!this->window) {
        auto error = fmt::format("Error creating window: {}", SDL_GetError());
        throw std::runtime_error(error);
        SDL_Quit();
    }

    // Create a renderer
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    if (!this->renderer) {
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        auto error = fmt::format("Error creating renderer: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    // Initialize SDL_ttf
    if(TTF_Init()) {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        SDL_Quit();
        auto error = fmt::format("Error initializing SDL_ttf: {}", TTF_GetError());
        throw std::runtime_error(error);
    }
}

Game::~Game() {
    // Clean up SDL_ttf and SDL2.
    TTF_Quit();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void Game::run() {

    // The Outer and Inter Bubble colors.
    SDL_Color outer_color = {200, 100, 150, 255};
    SDL_Color center_color = {50, 50, 150, 255};
    BubbleText bubbleText(this->renderer, "Bubble Text", 100, 20, outer_color, center_color);

    Fps fps(60);

    // Main game loop
    while (true) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            } else if (event.type == SDL_KEYDOWN) {
                // keyboard API for key pressed
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        return;
                    default:
                        break;
                }
            }
        }

        bubbleText.update();

        // Clear the screen
        SDL_RenderClear(this->renderer);

        bubbleText.draw();

        // Present the screen
        SDL_RenderPresent(this->renderer);

        fps.update();
    }
}