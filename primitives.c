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

void draw_digit(SDL_Surface *surface, char digit, int x, int y, int w, int dotRadius, Uint32 color) {
    //const int separation = 17;
    //const int cornerSeparation = 12;
    //const int radius = 8;
    const int separation = 15;
    const int cornerSeparation = 12;
    const int radius = dotRadius;
    const int segmentLength = 4;
    const int slopeStepPx = 2;

    const int maxWidth = radius + (cornerSeparation*2) + (separation*(segmentLength-1)) + radius;
    const int maxHeight = (radius*2) + (cornerSeparation/2) + (separation*(segmentLength)) + (separation*(segmentLength)) + radius;

    x = x - (maxWidth/2);
    y = y - (maxHeight/2);

    switch(digit) {
        case '0':
            digit = 0b0111111;
            break;
        case '1':
            digit = 0b0000110;
            break;
        case '2':
            digit = 0b1011011;
            break;
        case '3':
            digit = 0b1001111;
            break;
        case '4':
            digit = 0b1100110;
            break;
        case '5':
            digit = 0b1101101;
            break;
        case '6':
            digit = 0b1111101;
            break;
        case '7':
            digit = 0b0000111;
            break;
        case '8':
            digit = 0b1111111;
            break;
        case '9':
            digit = 0b1101111;
            break;
    }

    if(digit & 0b0000001) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius + cornerSeparation + separation*(i), y + radius, radius, color);
            }
    }
    if(digit & 0b0000010) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius + (cornerSeparation*2) + (separation*(segmentLength-1)), y + radius + cornerSeparation + separation*(i), radius, color);
            }
    }
    if(digit & 0b0000100) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius + (cornerSeparation*2) + (separation*(segmentLength-1)), y + (radius/2) + (cornerSeparation*2) + (separation*segmentLength) + (separation*i), radius, color);
            }
    }
    if(digit & 0b0001000) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius + cornerSeparation + separation*i, y + (radius*2) + (cornerSeparation) + (separation*(segmentLength)) + (separation*(segmentLength)), radius, color);
            }
    }
    if(digit & 0b0010000) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius, y + (radius/2) + (cornerSeparation*2) + (separation*segmentLength) + (separation*i), radius, color);
            }
    }
    if(digit & 0b0100000) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius, y + radius + cornerSeparation + (separation*(i)), radius, color);
            }
    }
    if(digit & 0b1000000) {
            for(int i = 0; i < segmentLength; i++) {
                draw_circle(surface, x + radius + cornerSeparation + separation*i, y + (radius/2) + cornerSeparation + (separation*(segmentLength)), radius, color);
            }
    }
}

void draw_digit_clock(SDL_Surface *surface, int cx, int cy, int w, int dotRadius, Uint32 color, struct tm *temps, int sec) {
    char tempsStr[5];
    int largeurDigit = w/4;

    if(temps == NULL) {
        strcpy(tempsStr, "8888");
    } else {
        strftime(tempsStr, 5, "%H%M", temps);
    }
    //printf("%s\n",tempsStr);

    draw_digit(surface, tempsStr[0], cx - (largeurDigit/2)*2 - largeurDigit - (largeurDigit/3), cy, w/4, dotRadius, color);
    draw_digit(surface, tempsStr[1], cx - (largeurDigit/2)*2, cy, w/4, dotRadius, color);

    if(temps == NULL || sec % 2) {
        draw_circle(surface, cx, cy - 30, dotRadius, color);
        draw_circle(surface, cx, cy + 30, dotRadius, color);
    }
    draw_digit(surface, tempsStr[2], cx + (largeurDigit/2)*2, cy, w/4, dotRadius, color);
    draw_digit(surface, tempsStr[3], cx + (largeurDigit/2)*2 + largeurDigit + (largeurDigit/3), cy, w/4, dotRadius, color);
}
