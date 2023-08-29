#include "bubble_text.h"
// #include "math.h"

bool bubble_text_new(
    struct Bubble_Text **bubble_text,
    SDL_Renderer *renderer,
    const char* text,
    int text_size,
    int radius,
    SDL_Color outer_color,
    SDL_Color center_color
) {
    *bubble_text = calloc(1, sizeof(struct Bubble_Text));
    if (!*bubble_text) {
        fprintf(stderr, "Error in calloc of bubble_text!\n");
        return true;
    }
    struct Bubble_Text *b = *bubble_text;

    b->renderer = renderer;

    b->font = TTF_OpenFont("fonts/freesansbold.ttf", text_size);
    if (!b->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    b->text_surf = TTF_RenderText_Blended(b->font, text, outer_color);
    if (!b->text_surf) {
        fprintf(stderr, "Error creating first TTF surface: %s\n", SDL_GetError());
        return true;
    }

    int padding = radius * 2;
    b->target_surf = SDL_CreateRGBSurface(0,b->text_surf->w + padding, b->text_surf->h + padding,32,0,0,0,0);
    if (!b->target_surf) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return true;
    }

    // Using the outer_color text to stamp in a circle.

    // Polar Coordinates trigonometry Algorithm
    // for (int index = 0; index < (int)(2 * M_PI * radius); index++) {
    //     double rad = (double)index / radius;
    //     int x = (int)(cos(rad) * radius) + radius;
    //     int y = (int)(sin(rad) * radius) + radius;
    //     SDL_BlitSurface(b->text_surf, NULL, b->target_surf, &(SDL_Rect){x, y, b->text_surf->w, b->text_surf->h });
    // }

    // Bresenham's Circle Drawing Algorithm
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    blit_symmetric_points(b->text_surf, b->target_surf, radius, x, y);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        blit_symmetric_points(b->text_surf, b->target_surf, radius, x, y);
    }

    SDL_FreeSurface(b->text_surf);
    b->text_surf = NULL;

    b->text_surf = TTF_RenderText_Blended(b->font, text, center_color);
    if (!b->text_surf) {
        fprintf(stderr, "Error creating second TTF surface: %s\n", SDL_GetError());
        return true;
    }

    SDL_BlitSurface(b->text_surf, NULL, b->target_surf, &(SDL_Rect){ radius, radius, b->text_surf->w, b->text_surf->h });

    b->image = SDL_CreateTextureFromSurface(b->renderer, b->target_surf);
    if (!b->image) {
        fprintf(stderr, "Error creating a text texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_QueryTexture(b->image, NULL, NULL, &b->rect.w, &b->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    b->speed = b->x_vel = b->y_vel = 120;

    return false;
}

void bubble_text_free(struct Bubble_Text **bubble_text) {
    TTF_CloseFont((*bubble_text)->font);
    (*bubble_text)->font = NULL;
    SDL_FreeSurface((*bubble_text)->text_surf);
    (*bubble_text)->text_surf = NULL;
    SDL_FreeSurface((*bubble_text)->target_surf);
    (*bubble_text)->target_surf = NULL;
    SDL_DestroyTexture((*bubble_text)->image);
    (*bubble_text)->image = NULL;
    (*bubble_text)->renderer = NULL;
    free(*bubble_text);
    *bubble_text = NULL;
}

void blit_symmetric_points(SDL_Surface *text_surf, SDL_Surface *target_surf, int radius, int x, int y) {
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius + y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + x, radius - y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius + y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - x, radius - y, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius + x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius + y, radius - x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius + x, text_surf->w, text_surf->h });
    SDL_BlitSurface(text_surf, NULL, target_surf, &(SDL_Rect){ radius - y, radius - x, text_surf->w, text_surf->h });
}

void bubble_text_update(struct Bubble_Text *b, double dt) {
    b->x_pos += b->x_vel * dt;
    b->rect.x = (int)(b->x_pos + 0.5);
    if (b->x_vel > 0) {
        if (b->rect.x + b->rect.w > WINDOW_WIDTH) {
            b->x_vel = -b->speed;
        }
    } else {
        if (b->rect.x < 0) {
            b->x_vel = b->speed;
        }  
    }
    b->y_pos += b->y_vel * dt;
    b->rect.y = (int)(b->y_pos + 0.5);
    if (b->y_vel > 0) {
        if (b->rect.y + b->rect.h > WINDOW_HEIGHT) {
            b->y_vel = -b->speed;
        }
    } else {
        if (b->rect.y < 0) {
            b->y_vel = b->speed;
        }  
    }
}

bool bubble_text_draw(struct Bubble_Text *b) {
    if (SDL_RenderCopy(b->renderer, b->image, NULL, &b->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}


