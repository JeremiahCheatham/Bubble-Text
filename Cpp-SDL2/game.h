#ifndef GAME_H
#define GAME_H

#include "main.h"

class Game {
public:
    Game();
    ~Game();

    void init();
    void run();

    static constexpr int width = 1280;
    static constexpr int height = 720;

private:
    const char* title = "Bubble Text";
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

#endif // GAME_H