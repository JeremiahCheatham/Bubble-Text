#include "bubble_text.h"

struct Bubble_Text* bubble_text_new(SDL_Renderer *renderer, const char* text, int text_size, int radius, SDL_Color outer_color, SDL_Color center_color) {
    struct Bubble_Text *this = calloc(1, sizeof(struct Bubble_Text));
    if (!this) {
        fprintf(stderr, "Error in calloc of bubble_text!\n");
        return this;
    }

    this->renderer = renderer;
    this->image = NULL;
    this->rect.x = 10;
    this->rect.y = 10;
    this->error = true;
    this->vel_x = 2;
    this->vel_y = 2;

    TTF_Font *font = TTF_OpenFont("fonts/freesansbold.ttf", text_size);
    if (!font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return this;
    }

    SDL_Surface *text_surf = TTF_RenderText_Blended(font, text, outer_color);
    if (!text_surf) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return this;
    }

    SDL_Surface *target_surf = SDL_CreateRGBSurface(0,text_surf->w + 20,text_surf->h + 20,32,0,0,0,0);
    if (!target_surf) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return this;
    }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int x = 0;
    int y = radius;
    int d = radius;
    while (y >= x) {
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius + y, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius - y, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius + y, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius - y, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius + x, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius - x, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius + x, text_surf->w, text_surf->h });
        SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius - x, text_surf->w, text_surf->h });
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
    SDL_FreeSurface(text_surf);
    text_surf = NULL;

    text_surf = TTF_RenderText_Blended(font, text, center_color);
    if (!text_surf) {
        fprintf(stderr, "Error creating second TTF surface: %s\n", SDL_GetError());
        return this;
    }

    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius, radius, text_surf->w, text_surf->h });

    this->image = SDL_CreateTextureFromSurface(this->renderer, target_surf);
    if (!this->image) {
        fprintf(stderr, "Error creating a text texture: %s\n", SDL_GetError());
        return false;
    }

    // Get a rect from the original image.
    if (SDL_QueryTexture(this->image, NULL, NULL, &this->rect.w, &this->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return false;
    }

    SDL_FreeSurface(text_surf);
    SDL_FreeSurface(target_surf);
    TTF_CloseFont(font);

    this->error = false;
    return this;
}

void bubble_text_update(struct Bubble_Text *this) {
    this->rect.x += this->vel_x;
    if (this->vel_x > 0) {
        if (this->rect.x + this->rect.w > WIDTH) {
            this->vel_x = -2;
        }
    } else {
        if (this->rect.x < 0) {
            this->vel_x = 2;
        }  
    }
    this->rect.y += this->vel_y;
    if (this->vel_y > 0) {
        if (this->rect.y + this->rect.h > HEIGHT) {
            this->vel_y = -2;
        }
    } else {
        if (this->rect.y < 0) {
            this->vel_y = 2;
        }  
    }
}

bool bubble_text_draw(struct Bubble_Text *this) {
    if (SDL_RenderCopy(this->renderer, this->image, NULL, &this->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void bubble_text_free(struct Bubble_Text *this) {
    if (this->image) {
        SDL_DestroyTexture(this->image);
    }
    free(this);
}
