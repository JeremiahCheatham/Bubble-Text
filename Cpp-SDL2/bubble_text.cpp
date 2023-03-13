#include "bubble_text.h"
#include "game.h"

BubbleText::BubbleText(SDL_Renderer* renderer, const char* text, int text_size, float radius, SDL_Color outer_color, SDL_Color center_color)
    : renderer(renderer) {

    // create font based on a true type font and size.
    TTF_Font *font = TTF_OpenFont("fonts/freesansbold.ttf", text_size);
    if (!font) {
        auto error = fmt::format("Error creating font: {}", TTF_GetError());
        throw std::runtime_error(error);
    }

    // Create initial Text to calculate target surface size.
    SDL_Surface *text_surf = TTF_RenderText_Blended(font, text, outer_color);
    if (!text_surf) {
        auto error = fmt::format("Error creating first TTF surface: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    // Create target surface with increased size for padding.
    int padding = radius * 2;
    SDL_Surface *target_surf = SDL_CreateRGBSurface(0,text_surf->w + padding,text_surf->h + padding,32,0,0,0,0);
    if (!text_surf) {
        auto error = fmt::format("Error creating target text surface: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->rect.w = text_surf->w;
    this->rect.h = text_surf->h;

    // Using the outer_color text to stamp in a circle.

    // Polar Coordinates trigonometry Algorithm
    // for (int index = 0; index < (int)(M_PI * radius); index++) {
    //     double x = (cos(index / (radius / 2)) * radius) + radius;
    //     double y = (sin(index / (radius / 2)) * radius) + radius;
    //     this->rect.x = x; this->rect.y = y;
    //     SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
    // }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int x = 0;
    int y = radius;
    int d = radius;
    while (y >= x) {
        this->rect.x = radius + x; this->rect.y = radius + y;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius + x; this->rect.y = radius - y;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius - x; this->rect.y = radius + y;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius - x; this->rect.y = radius - y;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius + y; this->rect.y = radius + x;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius + y; this->rect.y = radius - x;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius - y; this->rect.y = radius + x;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        this->rect.x = radius - y; this->rect.y = radius - x;
        SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }

    // Free text_surf before using it to create center text.
    SDL_FreeSurface(text_surf);
    text_surf = NULL;

    // Create Center Text center_color.
    text_surf = TTF_RenderText_Blended(font, text, center_color);
    if (!text_surf) {
        auto error = fmt::format("Error creating second TTF surface: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    // Using the center_color stamp the text in the center.
    this->rect.x = radius; this->rect.y = radius;
    SDL_BlitSurface(text_surf, NULL, target_surf, &this->rect);

    // Create a gpu accelerated texture image from the target surface.
    this->image = SDL_CreateTextureFromSurface(this->renderer, target_surf);
    if (!this->image) {
        auto error = fmt::format("Error creating a text texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    // Get a rect from the final texture image.
    if (SDL_QueryTexture(this->image, NULL, NULL, &this->rect.w, &this->rect.h)) {
        auto error = fmt::format("Error while querying texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }

    this->rect.x = 0;
    this->rect.y = 0;

    // free memory allocated for surfaces and font.
    SDL_FreeSurface(text_surf);
    SDL_FreeSurface(target_surf);
    TTF_CloseFont(font);
}

void BubbleText::update() {
    this->rect.x += this->vel_x;
    if (this->vel_x > 0) {
        if (this->rect.x + this->rect.w > Game::width) {
            this->vel_x = -2;
        }
    } else {
        if (this->rect.x < 0) {
            this->vel_x = 2;
        }  
    }
    this->rect.y += this->vel_y;
    if (this->vel_y > 0) {
        if (this->rect.y + this->rect.h > Game::height) {
            this->vel_y = -2;
        }
    } else {
        if (this->rect.y < 0) {
            this->vel_y = 2;
        }  
    }
}

BubbleText::~BubbleText() {
    SDL_DestroyTexture(this->image);
}

void BubbleText::draw() {
    if (SDL_RenderCopy(this->renderer, this->image, nullptr, &this->rect)) {
        auto error = fmt::format("Error while rendering texture: {}", SDL_GetError());
        throw std::runtime_error(error);
    }
}