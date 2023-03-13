#ifndef BUBBLE_TEXT_H
#define BUBBLE_TEXT_H

#include "main.h"

class BubbleText {
public:
    BubbleText(SDL_Renderer* renderer, const char* text, int size, float radius, SDL_Color outer_color, SDL_Color center_color);
    ~BubbleText();

    void update();
    void draw();

private:
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* image = nullptr;
    SDL_Rect rect;
    int vel_x = 2;
    int vel_y = 2;
};

#endif // BUBBLE_TEXT_H