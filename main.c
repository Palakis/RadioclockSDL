#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include "primitives.h"

int main ( int argc, char** argv )
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
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, screenBPP, screenFlags);
    if ( !screen )
    {
        printf("Impossible d'utiliser la résolution souhaitée: %s\n", SDL_GetError());
        return 1;
    }

    // On charge le logo CodeBlocks dans une surface
    SDL_Surface* bmp = SDL_LoadBMP("cb.bmp");
    if (!bmp)
    {
        printf("Erreur chargement BMP: %s\n", SDL_GetError());
        return 1;
    }

    // Coordonnées de placement de l'image, au centre de la surface d'affichage.
    SDL_Rect dstrect;

    // Couleurs des éléments visuels
    Uint32 couleurRoue = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 couleurCadran = SDL_MapRGB(screen->format, 255, 0, 0);
    Uint32 couleurRoueOff = SDL_MapRGB(screen->format, 32, 32, 32);

    // Rayons des éléments
    int rayonPoint = 8;
    int rayonHorloge = 200;
    int ecartCadran = 22;

    // Stockage du temps
    time_t tempsBrut;
    struct tm * temps;

    const int framerate = 15;
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

        // On calcule les coordonnées de l'image
        dstrect.x = (screen->w - bmp->w) / 2;
        dstrect.y = (screen->h - bmp->h) / 2;
        // On affiche l'image
        SDL_BlitSurface(bmp, 0, screen, &dstrect);

        // Calcul rayon horloge en fonction de la largeur de l'écran
        rayonHorloge = (screen->w / 2) - rayonPoint - ecartCadran;

        // Limitation rayon horloge en fonction de la hauteur de l'écran
        if(rayonHorloge > (screen->h / 2) - rayonPoint - ecartCadran) {
            rayonHorloge = (screen->h / 2) - rayonPoint - ecartCadran;
        }

        // Calcul rayon point et écart cadran
        rayonPoint = (rayonHorloge) / 25;
        ecartCadran = (rayonHorloge) / 9;

        // On récupère l'heure et la date
        time(&tempsBrut);
        temps = localtime(&tempsBrut);
        //printf("%d\n", temps->tm_sec);

        // On affiche la roue en mode "off"
        //draw_dots(screen, 60, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoueOff);
        // Puis on affiche le cadran autour de la roue
        //draw_dots(screen, 12, 12, (screen->w / 2) - (rayonHorloge + ecartCadran), (screen->h / 2) - (rayonHorloge + ecartCadran), (rayonHorloge + ecartCadran), rayonPoint, couleurCadran);
        // Et enfin on affiche la roue des secondes
        //draw_dots(screen, temps->tm_sec, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoue);

        //draw_circle(screen, rayonPoint+100, rayonPoint, rayonPoint, 0x00FF00);
        //draw_dots(screen, 60, 60, 100+rayonPoint, rayonPoint, rayonHorloge, rayonPoint, couleurRoueOff);
        //set_pixel(screen, 100, 50, 0xFFFFFF);
        draw_digit(screen, '2', 0, 0, 136, 0xFFFFFF);
        // FIN DE L'AFFICHAGE

        // Mise à jour de l'affichage
        SDL_Flip(screen);

        // Méthode dégueulasse pour restreindre le nombre de FPS
        // Par extension, réduit la conso CPU
        SDL_Delay(timeDelta);
    }

    // On décharge l'image du logo CodeBlocks
    SDL_FreeSurface(bmp);
    return 0;
}
