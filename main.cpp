#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>

#define r2d(a) (a * 57.295779513082)
#define d2r(a) (a * 0.017453292519)

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
void draw_dots(SDL_Surface *surface, int secondes, int totalSecondes, int x, int y, int radius, int dotRadius, Uint32 dotColor);

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // load an image
    SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        return 1;
    }

    // centre the bitmap on screen
    SDL_Rect dstrect;
    dstrect.x = (screen->w - bmp->w) / 2;
    dstrect.y = (screen->h - bmp->h) / 2;

    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
                    break;
                }
            } // end switch
        } // end of message processing

        // DRAWING STARTS HERE

        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // draw bitmap
        SDL_BlitSurface(bmp, 0, screen, &dstrect);

        time_t tempsBrut;
        struct tm * temps;
        time(&tempsBrut);
        temps = localtime(&tempsBrut);
        printf("%d\n", temps->tm_sec);
        draw_dots(screen, 0, 60, (screen->w / 2) - 200, (screen->h / 2) - 200, 200, 8, SDL_MapRGB(screen->format, 32, 32, 32));
        draw_dots(screen, temps->tm_sec, 60, (screen->w / 2) - 200, (screen->h / 2) - 200, 200, 8, SDL_MapRGB(screen->format, 255, 0, 0));
        draw_dots(screen, 0, 12, (screen->w / 2) - 222, (screen->h / 2) - 222, 222, 8, SDL_MapRGB(screen->format, 255, 0, 0));
        // DRAWING ENDS HERE

        // finally, update the screen :)
        SDL_Flip(screen);
    } // end main loop

    // free loaded bitmap
    SDL_FreeSurface(bmp);
    return 0;
}

void draw_circle(SDL_Surface *surface, int cx, int cy, int radius, Uint32 pixel) {
    static const int BPP = surface->format->BytesPerPixel;
    double r = (double)radius;

    for(double dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        int x = cx - dx;

        Uint8 *target_pixel_a = (Uint8 *)surface->pixels + ((int)(cy + r - dy)) * surface->pitch + x * BPP;
        Uint8 *target_pixel_b = (Uint8 *)surface->pixels + ((int)(cy - r + dy)) * surface->pitch + x * BPP;

        for(; x <= cx + dx; x++) {
            *(Uint32 *)target_pixel_a = pixel;
            *(Uint32 *)target_pixel_b = pixel;
            target_pixel_a += BPP;
            target_pixel_b += BPP;
        }
    }
}

void draw_dots(SDL_Surface *surface, int secondes, int totalSecondes, int x, int y, int radius, int dotRadius, Uint32 dotColor) {
    int centerX = x + radius;
    int centerY = y + radius;

    if(secondes > totalSecondes) {
        secondes = totalSecondes;
    }

    if(secondes < 0) {
        secondes = 0;
    }

    if(secondes == 0) {
        secondes = totalSecondes;
    }

    for(int i = 0; i < secondes; i++) {
        int dotX = centerX + radius * cos((d2r(360)/totalSecondes)*i-d2r(90));
        int dotY = centerY + radius * sin((d2r(360)/totalSecondes)*i-d2r(90));
        draw_circle(surface, dotX, dotY, dotRadius, dotColor);
    }
}
