#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>

// Fonctions de conversion radians/degrés
#define r2d(a) (a * 57.295779513082)
#define d2r(a) (a * 0.017453292519)

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 color);
void draw_circle(SDL_Surface *surface, int n_cx, int n_cy, int radius, Uint32 pixel);
void draw_dots(SDL_Surface *surface, int secondes, int totalSecondes, int x, int y, int radius, int dotRadius, Uint32 dotColor);

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
    Uint32 couleurRoue = SDL_MapRGB(screen->format, 0, 0, 255);
    Uint32 couleurCadran = SDL_MapRGB(screen->format, 0, 0, 255);
    Uint32 couleurRoueOff = SDL_MapRGB(screen->format, 32, 32, 32);

    // Rayons des éléments
    int rayonPoint = 8;
    int rayonHorloge = 200;

    // Stockage du temps
    time_t tempsBrut;
    struct tm * temps;

    // Boucle principale
    bool done = false;
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
                    done = true;
                    break;

                case SDL_KEYDOWN:
                {
                    // exit if ESCAPE is pressed
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        done = true;
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
        rayonHorloge = (screen->w / 2) - rayonPoint - 22;
        // Limitation rayon horloge en fonction de la hauteur de l'écran
        if(rayonHorloge > (screen->h / 2) - rayonPoint - 22) {
            rayonHorloge = (screen->h / 2) - rayonPoint - 22;
        }

        // On récupère l'heure et la date
        time(&tempsBrut);
        temps = localtime(&tempsBrut);
        printf("%d\n", temps->tm_sec);

        // On affiche la roue en mode "off"
        draw_dots(screen, 60, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoueOff);
        // Puis on affiche le cadran autour de la roue
        draw_dots(screen, 12, 12, (screen->w / 2) - (rayonHorloge + 22), (screen->h / 2) - (rayonHorloge + 22), (rayonHorloge + 22), rayonPoint, couleurCadran);
        // Et enfin on affiche la roue des secondes
        draw_dots(screen, temps->tm_sec, 60, (screen->w / 2) - rayonHorloge, (screen->h / 2) - rayonHorloge, rayonHorloge, rayonPoint, couleurRoue);

        //draw_circle(screen, rayonPoint+100, rayonPoint, rayonPoint, 0x00FF00);
        //draw_dots(screen, 60, 60, 100+rayonPoint, rayonPoint, rayonHorloge, rayonPoint, couleurRoueOff);
        //set_pixel(screen, 100, 50, 0xFFFFFF);

        // FIN DE L'AFFICHAGE

        // Mise à jour de l'affichage
        SDL_Flip(screen);
    }

    // On décharge l'image du logo CodeBlocks
    SDL_FreeSurface(bmp);
    return 0;
}

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
