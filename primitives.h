#ifndef PRIMITIVES_H_INCLUDED
#define PRIMITIVES_H_INCLUDED

#include <math.h>
#include <SDL/SDL.h>

// Fonctions de conversion radians/degrés
#define r2d(a) (a * 57.295779513082)
#define d2r(a) (a * 0.017453292519)

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
void draw_dots(SDL_Surface *surface, int secondes, int totalSecondes, int x, int y, int radius, int dotRadius, Uint32 dotColor);
void draw_digit(SDL_Surface *surface, char digit, int x, int y, int w, Uint32 color);

#endif // PRIMITIVES_H_INCLUDED
