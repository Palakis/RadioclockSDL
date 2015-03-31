#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef __APPLE__
#include "SDL.h"
#else
#include <SDL/SDL.h>
#endif

#include "primitives.h"

int main(int argc, char **argv)
{
    // On initialise la SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Erreur init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // On s'assure que SDL est quitté correctement à l'arrêt du prog
    atexit(SDL_Quit);

    // Création d'une fenêtre
    Uint32 screenFlags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE;
    int screenBPP = 32;
    SDL_Surface* screen = SDL_SetVideoMode(470, 470, screenBPP, screenFlags);
    if ( !screen )
    {
        printf("Impossible d'utiliser la résolution souhaitée: %s\n", SDL_GetError());
        return 1;
    }

    // Couleurs des éléments visuels
    Uint32 couleurRoue = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 couleurCadran = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 couleurRoueOff = SDL_MapRGB(screen->format, 32, 32, 32);
    Uint32 couleurDigits = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 couleurDigitsOff = SDL_MapRGB(screen->format, 32, 32, 32);

    // Rayons des éléments
    int rayonPoint = 6;
    int rayonHorloge = 294;
    int ecartCadran = 22;

    // Stockage du temps
    time_t tempsBrut;
    struct tm *temps;

    const int framerate = 10;
    int timeDelta = 1000/framerate;

    // Boucle principale
    int done = 0;
    while (!done)
    {
        // Traitement des évènements
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // Message ou pas message ?
            switch (event.type)
            {
                case SDL_VIDEORESIZE:
                    screen = SDL_SetVideoMode(event.resize.w, event.resize.h, screenBPP, screenFlags);
                    break;

                case SDL_QUIT:
                    done = 1;
                    break;

                case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = 1;
                    break;
                }
            }
        }

        // DEBUT DE L'AFFICHAGE

        // Vidange de la surface d'affichage
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        // Calcul rayon horloge en fonction de la largeur de l'écran (nb: 300)
        rayonHorloge = (screen->h / 2) - rayonPoint - (screen->h * 0.05);

        // Limitation rayon horloge en fonction de la hauteur de l'écran
        if(rayonHorloge > (screen->h / 2) - rayonPoint - ecartCadran) {
            rayonHorloge = (screen->h / 2) - rayonPoint - ecartCadran;
        }
        if(rayonHorloge > (screen->w / 2) - rayonPoint - ecartCadran) {
            rayonHorloge = (screen->w / 2) - rayonPoint - ecartCadran;
        }

        // Calcul rayon point
        rayonPoint = rayonHorloge * 0.023;
        // Calcul écart cadran
        ecartCadran = rayonPoint * 4;

        // On récupère l'heure et la date
        time(&tempsBrut);
        temps = localtime(&tempsBrut);
        //printf("%d\n", temps->tm_sec);

        // On affiche la roue en mode "off"
        draw_dots(screen, 60, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoueOff);
        // Puis on affiche le cadran autour de la roue
        draw_dots(screen, 12, 12, (screen->w / 2) - (rayonHorloge + ecartCadran), (screen->h / 2) - (rayonHorloge + ecartCadran), (rayonHorloge + ecartCadran), rayonPoint, couleurCadran);
        // Et enfin on affiche la roue des secondes
        draw_dots(screen, temps->tm_sec, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoue);

        draw_digit_clock(screen, (screen->w / 2), (screen->h / 2), rayonHorloge, rayonPoint, couleurDigitsOff, NULL);
        draw_digit_clock(screen, (screen->w / 2), (screen->h / 2), rayonHorloge, rayonPoint, couleurDigits, tempsBrut);
        // FIN DE L'AFFICHAGE

        // Mise à jour de l'affichage
        SDL_Flip(screen);

        // Méthode dégueulasse pour restreindre le nombre de FPS
        // Par extension, réduit la conso CPU
        SDL_Delay(timeDelta);
    }

    return 0;
}
