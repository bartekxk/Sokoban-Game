#include "funkcje.h"
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
// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
struct pozycja//struktura pozycji w rankingu
{
	int czas;
	int ruchy;
	pozycja *next;
	pozycja();
};
pozycja::pozycja()
{
	next = 0;
}
struct lista
{
	pozycja *first;
	void add(int czas, int ruchy);
	void show(int x,int **tab);
	lista();
};
lista::lista()
{
	first = 0;
}
void lista::add(int czas, int ruchy)
{
	pozycja *nowy = new pozycja;
	nowy->czas = czas;
	nowy->ruchy = ruchy;
	if (first == 0)
	{
		first = nowy;
	}
	else
	{
		pozycja*x = first;
		while (x->next)
		{
			x = x->next;
		}
		x->next = nowy;
		nowy->next = 0;
	}
}
void lista::show(int x,int **tab)
{
	pozycja * szukany = first;
	for (int i = 0; i < x; i++)
	{
		szukany = szukany->next;
	}
	tab[x][0] = szukany->czas;
	tab[x][1] = szukany->ruchy;
}
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};
bool wczytajplansze(int plansza[12][17], char nazwapliku[100])
{
	//0-wall 1-free 2-box 3-target 4-hero
		char znak;//zmiena do wczytania \n
		FILE * plik = fopen(nazwapliku, "r");
		if (plik == NULL)
		{
			fclose(plik);
			return 0;
		}
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				fread(&znak, sizeof(char), 1, plik);
				plansza[i][j] = znak-48;
			}
			fread(&znak, sizeof(char), 1, plik);
		}
		fclose(plik);
		return 1;
}
bool sprawdzczykoniec(int plansza[12][17])
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (plansza[i][j] == 3)return 0;
		}
	}
	return 1;
}
void showmenu(int &pos,bool myszka,bool wielegier, SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer,double worldTime, double fps, char text[128],int kolor1,int kolor2)
{
	char pomocnicza[5],strzalka[5];
	sprintf(strzalka,"->");
	DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, kolor1, kolor2);
	sprintf(text, "Menu");
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
	if (pos == 0)sprintf(strzalka, "-> ");
	else sprintf(strzalka, ""); 
	sprintf(text, "%s Wczytaj plansze",strzalka);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
	if (pos == 1)sprintf(strzalka, "-> ");
	else sprintf(strzalka, "");
	sprintf(text, "%s Dodaj plansze",strzalka);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);
	if (pos == 2)sprintf(strzalka, "-> ");
	else sprintf(strzalka, "");
	if(wielegier==1)sprintf(pomocnicza, "On");
	else sprintf(pomocnicza, "Off");
	sprintf(text, "%s Rozegranie wielu gier: %s", strzalka,pomocnicza);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 58, text, charset);
	if (pos == 3)sprintf(strzalka, "-> ");
	else sprintf(strzalka, "");
	sprintf(text, "%s Najlepsze wyniki",strzalka);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 74, text, charset);
	if (pos == 4)sprintf(strzalka, "-> ");
	else sprintf(strzalka, "");
	if (myszka)sprintf(pomocnicza, "On");
	else sprintf(pomocnicza, "Off");
	sprintf(text, "%s Tryb myszki:%s", strzalka,pomocnicza);
	DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2,90, text, charset);
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void wczytajtekst(char nazwapliku[100], SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2)
{
	SDL_Event event;
	bool koniecpliku = 0;
	sprintf(nazwapliku, "");//czyscimy 
	do
	{
	while (SDL_PollEvent(&event))
	{
		DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, kolor1, kolor2);
		DrawString(screen, screen->w / 2 - strlen(nazwapliku) * 8 / 2, 10, nazwapliku, charset);
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_RETURN)koniecpliku = 1;//jesli nacisniemy enter konczymy wczytywac plik
			else
			{
				char x = event.key.keysym.sym;
				sprintf(nazwapliku, "%s%c", nazwapliku, x);
			}
		}
	}
	} while (!koniecpliku);

}
bool wybierzpoziom(char nazwapliku[100], SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2)
{
	SDL_Event event;
	int pos = 1,ile;
	char strzalka[5];
	sprintf(strzalka, "->");
	FILE * plik = fopen("config.txt", "r");
	fscanf(plik, "%d", &ile);
	char ** tab = (char**)malloc(ile*(sizeof(char*)));//robimy dynamiczna tablice wskaznikow na wskaznik
	for (int i = 0; i < ile; i++)
	{
		tab[i] = new char[100];
	}
	char znak;
	fscanf(plik, "%c", &znak);
	for (int i = 0; i < ile; i++)
	{
		fscanf(plik, "%s", tab[i]);
		fscanf(plik, "%c", &znak);//pomijanie znaku \n
	}
	for (;;)
	{
		DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, kolor1, kolor2);
		for (int i = 0; i < ile; i++)
		{
			if (pos - 1 == i)sprintf(text, "%s%s", strzalka, tab[i]);
			else sprintf(text, "%s", tab[i]);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10+(i*16), text, charset);
		}
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{

			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_m)
				{
					free(tab);
					fclose(plik);
					return 0;
				}
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					sprintf(nazwapliku, "%s.txt", tab[pos - 1]);
					free(tab);
					fclose(plik);
					return 1;
				}
				if (event.key.keysym.sym == SDLK_DOWN)//poruszanie sie po menu
				{
					if (pos == ile)pos = 1;
					else pos++;
				}
				if (event.key.keysym.sym == SDLK_UP)
				{
					if (pos == 1)pos = ile;
					else pos--;
				}
			}
		}
	}

}
void sortuj(int **tab,int ile, int x)
{
	int a, b,z;//do przepisania
	do {
		z = 0;
		for (int i = 0; i < ile - 1; i++)
		{
			if (tab[i][x] > tab[i + 1][x])
			{
				z++;
				a=tab[i][0];
				b = tab[i][1];
				tab[i][0] = tab[i + 1][0];
				tab[i][1] = tab[i + 1][1];
				tab[i + 1][0]= a;
				tab[i + 1][1] = b;
			}
		}
	} while (z != 0);
}
void ranking(SDL_Texture *scrtex, SDL_Surface *screen, SDL_Surface *charset, SDL_Renderer *renderer, double worldTime, double fps, char text[128], int kolor1, int kolor2)
{
	SDL_Event event;
	lista *rkg = new lista;
	int ile = 0;
	char rankingpoziomu[100];
	bool ranking = 0;//0-wedlug czasu 1-wedlug ruchow
	char znak;//zmiena do wczytania planszy
	if (wybierzpoziom(rankingpoziomu, scrtex, screen, charset, renderer, worldTime, fps, text, kolor1, kolor2) == 0)return;//wybieramy poziom dla ktorego wyswitlimy ranking
	FILE * plik = fopen(rankingpoziomu, "r");//wczytujemy plik planszy i rankingu
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			fread(&znak, sizeof(char), 1, plik);
		}
		fread(&znak, sizeof(char), 1, plik);
	}//przechodzimy cala plansze bo nasz ranking jest za nia
	int x, y;//zmienna do wczytywania danych rankingu
	for (;;)
	{
		if (fscanf(plik, "%d%d", &x, &y) == EOF)break;
		rkg->add(x, y);//dodajemy do listy
		ile++;
	}//czytamy do konca pliku ranking
	fclose(plik);
	//zrobimy tablice do ktorej przepiszemy liste
	int ** tab = (int**)malloc(ile*(sizeof(int*)));//robimy dynamiczna tablice wskaznikow na wskaznik
	for (int i = 0; i < ile; i++)
	{
		tab[i] = new int[2];
		rkg->show(i, tab);
	}
	for (;;)//wyswietlamy liste i oczekujemy przeciskow
	{
		DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, kolor1, kolor2);
		if (ranking)sprintf(text, "%s", "->Ranking wedlug ruchow");
		else sprintf(text, "%s", "->Ranking wedlug czasu");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		if (ranking)sortuj(tab,ile, 1);
		else sortuj(tab,ile, 0);
		for (int i = 0; i < ile; i++)
		{
			sprintf(text, "%d %d", tab[i][0], tab[i][1]);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26 + (i * 16), text, charset);
		}
		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{

			switch (event.type) {
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_m)
				{
					free(tab);
					return;
				}
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					ranking = !ranking;
				}
			}
		}
	}
}
void dodajdorankingu(char nazwapliku[100], int wykonaneruchy, int czas)
{
	FILE * plik = fopen(nazwapliku, "a");
	fprintf(plik, "\n%d %d", czas, wykonaneruchy);
	fclose(plik);
}