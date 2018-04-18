#ifndef funkcje_h
#define funkcje_h
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include "funkcje.h"
#include<string.h>
#include <time.h>
extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#define SCREEN_WIDTH	646
#define SCREEN_HEIGHT	489
#define Boxsize 38
// narysowanie napisu txt na powierzchni screen, zaczynaj¹c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj¹ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,SDL_Surface *charset);
// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt œrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
// rysowanie linii o d³ugoœci l w pionie (gdy dx = 0, dy = 1) 
// b¹dŸ poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
// rysowanie prostok¹ta o d³ugoœci boków l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor);
bool wczytajplansze(int plansza[12][17], char nazwapliku[100]);
bool sprawdzczykoniec(int plansza[12][17]);
void showmenu(int &pos,bool myszka,bool wielegier, SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2);
void wczytajtekst(char nazwapliku[100], SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2);
bool wybierzpoziom(char nazwapliku[100], SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2);
void sortuj(int **tab,int ile, int x);
void ranking(SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2);
void dodajdorankingu(char nazwapliku[100], int wykonaneruchy, int czas);
#endif