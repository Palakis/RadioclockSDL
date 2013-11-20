#include "primitives.h"

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    if(x < surface->w && y < surface->h && x > 0 && y > 0) {
        Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
        *(Uint32 *)target_pixel = color;
        target_pixel += surface->format->BitsPerPixel;
    }
}

void draw_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel) {
    double r = (double)radius;

    for(double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - dx;

        for(; x <= cx + dx; x++) {
            set_pixel(surface, x, ((int)(cy + r - dy)), pixel);
            set_pixel(surface, x, ((int)(cy - r + dy)), pixel);
        }
    }
}

void draw_dots(SDL_Surface *surface, int secondes, int totalSecondes, int x, int y, int radius, int dotRadius, Uint32 dotColor) {
    int centerX = x + radius;
    int centerY = y + radius;
    secondes++;

    if(secondes > totalSecondes) {
        secondes = totalSecondes;
    }

    for(int i = 0; i < secondes; i++) {
        int dotX = centerX + radius * cos((d2r(360)/totalSecondes)*i-d2r(90));
        int dotY = centerY + radius * sin((d2r(360)/totalSecondes)*i-d2r(90));
        draw_circle(surface, dotX, dotY, dotRadius, dotColor);
    }
}
