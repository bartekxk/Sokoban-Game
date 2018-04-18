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


// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	int t1, t2, quit, frames, rc,koniect,wykonaneruchy;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	SDL_Event event;
	SDL_MouseButtonEvent mevent;
	SDL_Surface *screen, *charset;
	SDL_Surface *box, *free, *hero, *target,*koniec, *wall;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	// okno konsoli nie jest widoczne, je¿eli chcemy zobaczyæ
	// komunikaty wypisywane printf-em trzeba w opcjach:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmieniæ na "Console"
	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}

	// tryb pe³noekranowy / fullscreen mode
	//rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
                             //    &window, &renderer);
	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);
	if (rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_SetWindowTitle(window, "Bart³omiej Kocot 171557");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);


	// wy³¹czenie widocznoœci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	SDL_SetColorKey(charset, true, 0x000000);

	box = SDL_LoadBMP("./box.bmp");
	hero = SDL_LoadBMP("./hero.bmp");
	free = SDL_LoadBMP("./free.bmp");
	target = SDL_LoadBMP("./target.bmp");
	wall = SDL_LoadBMP("./wall.bmp");
	koniec = SDL_LoadBMP("./kg.bmp");
	if (target == NULL || free == NULL || hero == NULL || box == NULL || wall == NULL || koniec==NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	char text[128],nazwapliku[100];
	int x, y;//polozenie bohatera
	int zapamietaj = 1;//pamietamy pole na ktorym jestesmy
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
	int plansza[12][17];//tablica na plansze
	bool newgame, wielegier = 1,myszka=0;
	sprintf(nazwapliku, "random.txt");
	do
	{
		newgame = 0;
		wykonaneruchy = 0;
		if (wczytajplansze(plansza,nazwapliku) == 0)return 0;//wczytujemy domyslna plansze
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 17; j++)
			{
				if (plansza[i][j] == 4)
				{
					x = i;
					y = j;//szukamy bohatera na planszy
				}
			}
		}
		t1 = SDL_GetTicks();

		frames = 0;
		fpsTimer = 0;
		fps = 0;
		quit = 0;
		worldTime = 0;
		distance = 0;
		etiSpeed = 1;
		koniect = 0;
		do {
			t2 = SDL_GetTicks();

			// w tym momencie t2-t1 to czas w milisekundach,
			// jaki uplyna³ od ostatniego narysowania ekranu
			// delta to ten sam czas w sekundach
			// here t2-t1 is the time in milliseconds since
			// the last screen was drawn
			// delta is the same time in seconds
			delta = (t2 - t1) * 0.001;
			t1 = t2;

			worldTime += delta;

			distance += etiSpeed * delta;

			SDL_FillRect(screen, NULL, czarny);

			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < 17; j++)
				{
					if (plansza[i][j] == 0)DrawSurface(screen, wall, 19 + (j*Boxsize), 52 + (i*Boxsize));
					if (plansza[i][j] == 1)DrawSurface(screen, free, 19 + (j*Boxsize), 52 + (i*Boxsize));
					if (plansza[i][j] == 2)DrawSurface(screen, box, 19 + (j*Boxsize), 52 + (i*Boxsize));
					if (plansza[i][j] == 3)DrawSurface(screen, target, 19 + (j*Boxsize), 52 + (i*Boxsize));
					if (plansza[i][j] == 4)DrawSurface(screen, hero, 19 + (j*Boxsize), 52 + (i*Boxsize));
					if (plansza[i][j] == 5)DrawSurface(screen, box, 19 + (j*Boxsize), 52 + (i*Boxsize));//box on target
				}
			}
			//sprawdzanie zakonczenia
			if (zapamietaj != 3 && sprawdzczykoniec(plansza) == 1)
			{
				if (koniect == 0)dodajdorankingu(nazwapliku, wykonaneruchy, worldTime);
				koniect++;
				DrawSurface(screen, koniec,  SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
				if (koniect == 500)
				{
					newgame = 1;
					if (wielegier == 0)
					{
						SDL_FreeSurface(charset);
						SDL_FreeSurface(screen);
						SDL_DestroyTexture(scrtex);
						SDL_DestroyRenderer(renderer);
						SDL_DestroyWindow(window);

						SDL_Quit();
						return 0;
					}
				}
			}
			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			};

			// tekst informacyjny / info text
			DrawRectangle(screen, 0, 0, SCREEN_WIDTH, 34, czerwony, niebieski);
			//            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
			sprintf(text, "Sokoban Bart³omiej Kocot, czas trwania = %.1lf s  %.0lf klatek / s", worldTime, fps);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);


			SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
			//		SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, scrtex, NULL, NULL);
			SDL_RenderPresent(renderer);

			// obs³uga zdarzeñ (o ile jakieœ zasz³y) / handling of events (if there were any)
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
					if (event.key.keysym.sym == SDLK_n) newgame = 1;
					if (event.key.keysym.sym == SDLK_m)
					{
						bool quitmenu = 0;
						int pos = 0;//pozycja w menu
						do {
							
							showmenu(pos,myszka,wielegier,scrtex,screen,charset,renderer,worldTime,fps,text, czerwony,niebieski);
							while (SDL_PollEvent(&event))
							{
								switch (event.type) {
								case SDL_KEYDOWN:
									if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_m) quitmenu = 1;
									if (event.key.keysym.sym == SDLK_DOWN)//poruszanie sie po menu
									{
										if (pos == 4)pos = 0;
										else pos++;
									}
									if (event.key.keysym.sym == SDLK_UP)
									{
										if (pos == 0)pos = 4;
										else pos--;
									}
									if (event.key.keysym.sym == SDLK_RETURN)//w przypadku enter sprawdzamy na ktorej jestesmy pozycji i robimy to co wybral gracz
									{
										if (pos == 0)
										{
											if(wybierzpoziom(nazwapliku,scrtex, screen, charset, renderer, worldTime, fps, text, czerwony, niebieski)==1)
											{
												newgame = 1;
												quitmenu = 1;
											}
										}
										if (pos == 1)
										{
											sprintf(nazwapliku, "");
											wczytajtekst(nazwapliku, scrtex, screen, charset, renderer, worldTime, fps, text, czerwony, niebieski);
											if (wczytajplansze(plansza, nazwapliku))
											{
												newgame = 1;
												quitmenu = 1;
											}
										}
										if (pos == 2)
										{
											wielegier = !wielegier;
										}
										if (pos == 3)
										{
											ranking(scrtex, screen, charset, renderer, worldTime, fps, text, czerwony, niebieski);
										}
										if (pos == 4)
										{
											if(myszka)SDL_ShowCursor(SDL_DISABLE);
											else SDL_ShowCursor(SDL_ENABLE);
											myszka = !myszka;
										}
									}
								}
							}
						} while (!quitmenu);
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						if (x - 1 >= 0 && (plansza[x - 1][y] == 1 || plansza[x - 1][y] == 3))
						{
							if (zapamietaj == 5)zapamietaj = 3;
							plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
							zapamietaj = plansza[x - 1][y];
							plansza[x - 1][y] = 4;
							x--;
							wykonaneruchy++;
						}
						else
							if (x - 2 >= 0 && (plansza[x - 1][y] == 2 || plansza[x - 1][y] == 5) && (plansza[x - 2][y] == 1 || plansza[x - 2][y] == 3))
							{
								if (zapamietaj == 5)zapamietaj = 3;
								if (plansza[x - 2][y] == 3)
								{
									plansza[x - 2][y] = 5;//box on target
								}
								else
									plansza[x - 2][y] = 2;
								plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
								if (plansza[x - 1][y] == 5)zapamietaj = 5;
								else zapamietaj = 1;
								plansza[x - 1][y] = 4;
								x--;
								wykonaneruchy++;
							}
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						if (x + 1 < 12 && (plansza[x + 1][y] == 1 || plansza[x + 1][y] == 3))
						{
							if (zapamietaj == 5)zapamietaj = 3;

							plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
							zapamietaj = plansza[x + 1][y];
							plansza[x + 1][y] = 4;
							x++;
							wykonaneruchy++;
						}
						else
							if (x + 2 < 12 && (plansza[x + 1][y] == 2 || plansza[x + 1][y] == 5) && (plansza[x + 2][y] == 1 || plansza[x + 2][y] == 3))
							{
								if (zapamietaj == 5)zapamietaj = 3;
								if (plansza[x + 2][y] == 3)
								{
									plansza[x + 2][y] = 5;//box on target
								}
								else
									plansza[x + 2][y] = 2;
								plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
								if (plansza[x + 1][y] == 5)zapamietaj = 5;
								else zapamietaj = 1;
								plansza[x + 1][y] = 4;
								x++;
								wykonaneruchy++;
							}
					}
					else if (event.key.keysym.sym == SDLK_LEFT)
					{
						if (y - 1 > 0 && (plansza[x][y - 1] == 1 || plansza[x][y - 1] == 3))
						{
							if (zapamietaj == 5)zapamietaj = 3;

							plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
							zapamietaj = plansza[x][y - 1];
							plansza[x][y - 1] = 4;
							y--;
							wykonaneruchy++;
						}
						else
							if (y - 2 > 0 && (plansza[x][y - 1] == 2 || plansza[x][y - 1] == 5) && (plansza[x][y - 2] == 1 || plansza[x][y - 2] == 3))
							{
								if (zapamietaj == 5)zapamietaj = 3;
								if (plansza[x][y - 2] == 3)
								{
									plansza[x][y - 2] = 5;//box on target
								}
								else
									plansza[x][y - 2] = 2;
								plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
								if (plansza[x][y - 1] == 5)zapamietaj = 5;
								else zapamietaj = 1;
								plansza[x][y - 1] = 4;
								y--;
								wykonaneruchy++;
							}
					}
					else if (event.key.keysym.sym == SDLK_RIGHT)
					{
						if (y + 1 < 17 && (plansza[x][y + 1] == 1 || plansza[x][y + 1] == 3))
						{
							if (zapamietaj == 5)zapamietaj = 3;

							plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
							zapamietaj = plansza[x][y + 1];
							plansza[x][y + 1] = 4;
							y++;
							wykonaneruchy++;
						}
						else
							if (y + 2 < 17 && (plansza[x][y + 1] == 2 || plansza[x][y + 1] == 5) && (plansza[x][y + 2] == 1 || plansza[x][y + 2] == 3))
							{
								if (zapamietaj == 5)zapamietaj = 3;
								if (plansza[x][y + 2] == 3)
								{
									plansza[x][y + 2] = 5;//box on target
								}
								else
									plansza[x][y + 2] = 2;
								plansza[x][y] = zapamietaj;//przywracamy poprzednie pole
								if (plansza[x][y + 1] == 5)zapamietaj = 5;
								else zapamietaj = 1;
								plansza[x][y + 1] = 4;
								y++;
								wykonaneruchy++;
							}

					}

					break;
				case SDL_QUIT:
					quit = 1;
					break;
				};
			};
			frames++;

		}while(!newgame && !quit);
	}while (!quit);

	// zwolnienie powierzchni / freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
