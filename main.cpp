#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

extern "C" {
#include"../template-project-2/SDL2-2.0.10/include/SDL.h"
#include"../template-project-2/SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define SZEROKOSC_ZAWIJANIA 1280
#define RUCH_PIKSELE 33

#define VICTORY_LANE 66
#define ROW_10 VICTORY_LANE + RUCH_PIKSELE
#define ROW_9 ROW_10 + RUCH_PIKSELE
#define ROW_8 ROW_9 + RUCH_PIKSELE
#define ROW_7 ROW_8 + RUCH_PIKSELE
#define ROW_6 ROW_7 + RUCH_PIKSELE
#define ROW_5 ROW_6 + 2*RUCH_PIKSELE
#define ROW_4 ROW_5 + RUCH_PIKSELE
#define ROW_3 ROW_4 + RUCH_PIKSELE
#define ROW_2 ROW_3 + RUCH_PIKSELE
#define ROW_1 ROW_2 + RUCH_PIKSELE

#define MOVABLES_IN_ROW_COUNT 5
#define MOVABLES_COUNT 50
#define OBSTACLE_ROWS_COUNT 10

#define MOVABLE_STANDARD_WIDTH 50
#define FROG_WIDTH 32
#define ROZNICA_STARTOWA 64
#define CAR_SPEED 250
#define BAL_SPEED 150
#define TURTLE_SPEED -50

#define INITIAL_FROG_X 320
#define INITIAL_FROG_Y 462
#define INITIAL_LIVES 5
#define ROUND_TIME 50
#define NUMBER_OF_HIGH_SCORES 10

void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset) {
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

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};


void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

int CheckTextures(SDL_Surface* charset, SDL_Surface* menu, SDL_Surface* screen, SDL_Texture* scrtex, SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* mapa, SDL_Surface* frog, SDL_Surface* carR, SDL_Surface* carB, SDL_Surface* turtle, SDL_Surface* bal)
{
	if (charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (menu == NULL) {
		printf("SDL_LoadBMP(menu.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (mapa == NULL) {
		printf("SDL_LoadBMP(mapa.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (frog == NULL) {
		printf("SDL_LoadBMP(frog.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (carR == NULL) {
		printf("SDL_LoadBMP(carR.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (carB == NULL) {
		printf("SDL_LoadBMP(carB.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (turtle == NULL) {
		printf("SDL_LoadBMP(zolw.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	if (bal == NULL) {
		printf("SDL_LoadBMP(bal.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(charset);
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	return 0;
}

void randomowe(int tablica[], int ile_wylosowac, int nr_losu = 0) {

	int pole_startowe = rand() % (SZEROKOSC_ZAWIJANIA / ROZNICA_STARTOWA);
	int pozycja = pole_startowe * ROZNICA_STARTOWA;


	for (int i = 0; i < nr_losu; i++) {
		if (tablica[i] == pozycja) {
			randomowe(tablica, ile_wylosowac, nr_losu);
			return;
		}
	}
	tablica[nr_losu] = pozycja;

	if (nr_losu < ile_wylosowac - 1) {
		randomowe(tablica, ile_wylosowac, nr_losu + 1);
	}
}

typedef struct movable_stc
{
	double x;
	int y;
	SDL_Surface* bitmap;
	int speed;
	bool lethal;

} Movable;

typedef struct is
{
	bool pause = true;
	bool quit_request = false;
	bool has_lost = false;
	bool typed_name_for_highscores = false;
	bool is_in_menu = true;
	bool is_in_highscores = false;
	int quit = 0;

} GameState;


bool collides(double frogX, int frogY, Movable movable)
{
	return abs((int)(frogX - movable.x)) < (FROG_WIDTH + MOVABLE_STANDARD_WIDTH) / 2 && frogY == movable.y;
}

void generateRow(SDL_Surface* sprite, int row, int speed, Movable* movables, bool lethal, int from)
{
	int startowe[MOVABLES_IN_ROW_COUNT];
	randomowe(startowe, MOVABLES_IN_ROW_COUNT);

	for (int i = from, si = 0; i < from + MOVABLES_IN_ROW_COUNT; i++, si++)
	{
		movables[i].x = startowe[si];
		movables[i].y = row;
		movables[i].bitmap = sprite;
		movables[i].speed = speed;
		movables[i].lethal = lethal;
	}
}

void generateSet(SDL_Surface* carR, SDL_Surface* carB, SDL_Surface* turtle, SDL_Surface* bal, Movable* objects)
{
	generateRow(carR, ROW_1, CAR_SPEED * 8 / 10, objects, true, MOVABLES_IN_ROW_COUNT * 0);
	generateRow(carB, ROW_2, -CAR_SPEED * 9 / 10, objects, true, MOVABLES_IN_ROW_COUNT * 1);
	generateRow(carR, ROW_3, CAR_SPEED, objects, true, MOVABLES_IN_ROW_COUNT * 2);
	generateRow(carB, ROW_4, -CAR_SPEED * 11 / 10, objects, true, MOVABLES_IN_ROW_COUNT * 3);
	generateRow(carR, ROW_5, CAR_SPEED * 12 / 10, objects, true, MOVABLES_IN_ROW_COUNT * 4);
	generateRow(turtle, ROW_6, TURTLE_SPEED, objects, false, MOVABLES_IN_ROW_COUNT * 5);
	generateRow(bal, ROW_7, BAL_SPEED, objects, false, MOVABLES_IN_ROW_COUNT * 6);
	generateRow(turtle, ROW_8, TURTLE_SPEED, objects, false, MOVABLES_IN_ROW_COUNT * 7);
	generateRow(bal, ROW_9, BAL_SPEED, objects, false, MOVABLES_IN_ROW_COUNT * 8);
	generateRow(turtle, ROW_10, TURTLE_SPEED, objects, false, MOVABLES_IN_ROW_COUNT * 9);
}

void DrawFrame(SDL_Texture* scrtex, SDL_Surface* screen, SDL_Renderer* renderer)
{
	SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
	SDL_RenderCopy(renderer, scrtex, NULL, NULL);
	SDL_RenderPresent(renderer);
}


int GameOver(SDL_Surface* screen, SDL_Surface* charset, int points, int lives)
{
	if (lives <= 0)
	{
		char text[50];
		sprintf_s(text, 50, "GAME OVER \n\n\n POINTS SCORED: %0d", points);
		DrawRectangle(screen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (0x00, 0xFF, 0x00), (0x00, 0xFF, 0x00));
		DrawString(screen, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2, text, charset);

		char text2[100] = "DO YOU WANT TO QUIT? Y/N";
		DrawString(screen, 250, (SCREEN_HEIGHT / 2) + 40, text2, charset);

		return 1;
	}
	else
		return 0;
}

void LosingLife(SDL_Surface* screen, SDL_Surface* charset, double* frogX, int* frogY, int* lives)
{
	(*lives)--;
	*frogX = INITIAL_FROG_X;
	*frogY = INITIAL_FROG_Y;

}

void DrawTime(SDL_Surface* screen, int x, int y, double* delta, SDL_Surface* charset, int* lives, double* frogX, int* frogY)
{

	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);

	int pasek = (int)(ROUND_TIME - *delta) * 4;

	char* time = "TIME:";
	DrawString(screen, x, y, time, charset);

	if (pasek > 40)
		DrawRectangle(screen, x + 40, y, pasek, 20, zielony, zielony);
	else if (pasek > 0)
		DrawRectangle(screen, x + 40, y, pasek, 20, czerwony, czerwony);
	else
	{
		*delta = 0;
		LosingLife(screen, charset, frogX, frogY, lives);
	}

}

void DrawLives(SDL_Surface* screen, SDL_Surface* charset, int lives)
{
	char text[20];
	sprintf_s(text, 20, "LIVES: %.0d", lives);

	DrawString(screen, 500, 10, text, charset);
}

void DrawPoints(SDL_Surface* screen, SDL_Surface* charset, int points)
{
	char text[20];
	sprintf_s(text, 20, "POINTS: %d", points);

	DrawString(screen, 50, 10, text, charset);
}

void FrogRescued(bool* czy_pelne, int* points, double* time)
{
	*czy_pelne = true;
	*points += (int)(50 + (ROUND_TIME - *time) * 10);
	*time = 0;
}

void VictoryLaneCheck(SDL_Surface* screen, SDL_Surface* charset, bool czy_pelne[5], double* frogX, int* frogY, int* points, double* time, int* lives)
{
	if (*frogY == VICTORY_LANE) {
		if (*frogX >= 34 && *frogX <= 61 && czy_pelne[0] == false)
		{
			FrogRescued(&czy_pelne[0], points, time);
		}
		else if (*frogX >= 171 && *frogX <= 198 && czy_pelne[1] == false)
		{
			FrogRescued(&czy_pelne[1], points, time);
		}
		else if (*frogX >= 308 && *frogX <= 335 && czy_pelne[2] == false)
		{
			FrogRescued(&czy_pelne[2], points, time);
		}
		else if (*frogX >= 445 && *frogX <= 472 && czy_pelne[3] == false)
		{
			FrogRescued(&czy_pelne[3], points, time);
		}
		else if (*frogX >= 582 && *frogX <= 609 && czy_pelne[4] == false)
		{
			FrogRescued(&czy_pelne[4], points, time);
		}
		else
		{
			LosingLife(screen, charset, frogX, frogY, lives);
		}
		*frogX = INITIAL_FROG_X;
		*frogY = INITIAL_FROG_Y;
	}
}

void DrawAtFinish(SDL_Surface* screen, SDL_Surface* frog, bool czy_pelne[5])
{
	if (czy_pelne[0] == true)
	{
		DrawSurface(screen, frog, 48, VICTORY_LANE);
	}

	if (czy_pelne[1] == true)
	{
		DrawSurface(screen, frog, 185, VICTORY_LANE);
	}

	if (czy_pelne[2] == true)
	{
		DrawSurface(screen, frog, 322, VICTORY_LANE);
	}
	if (czy_pelne[3] == true)
	{
		DrawSurface(screen, frog, 459, VICTORY_LANE);
	}
	if (czy_pelne[4] == true)
	{
		DrawSurface(screen, frog, 596, VICTORY_LANE);
	}
}

void IsAnythingEmpty(bool czy_pelne[5])
{
	if (czy_pelne[0] == true && czy_pelne[1] == true && czy_pelne[2] == true && czy_pelne[3] == true && czy_pelne[4] == true)
	{

		for (int i = 0; i <= 4; i++)
		{
			czy_pelne[i] = false;
		}
		Sleep(1000); // ms
	}
}

void IsFrogOutsideTheMap(SDL_Surface* screen, SDL_Surface* charset, double* frogX, int* frogY, int* lives)
{
	if (*frogX < 0 || *frogX > SCREEN_WIDTH || *frogY > SCREEN_HEIGHT)
	{
		LosingLife(screen, charset, frogX, frogY, lives);
	}
}

void MovablesMoveAndCollision(SDL_Surface* screen, SDL_Surface* charset, Movable* movables, double* frogX, int* frogY, int* lives, double delta, bool* floats)
{
	for (int i = 0; i < MOVABLES_COUNT; i++) {
		movables[i].x += movables[i].speed * delta;

		if (movables[i].x > SZEROKOSC_ZAWIJANIA - 0.5 * SCREEN_WIDTH)
		{
			movables[i].x -= SZEROKOSC_ZAWIJANIA;
		}
		else if (movables[i].x < -0.5 * SCREEN_WIDTH)
		{
			movables[i].x += SZEROKOSC_ZAWIJANIA;
		}
		DrawSurface(screen, movables[i].bitmap, movables[i].x, movables[i].y);

		if (collides(*frogX, *frogY, movables[i])) {
			if (movables[i].lethal) {
				LosingLife(screen, charset, frogX, frogY, lives);
			}
			else {
				*frogX += (movables[i].speed * delta);
				*floats = true;
			}
		}
	}
}

void IsFrogInWater(SDL_Surface* screen, SDL_Surface* charset, double* frogX, int* frogY, int* lives, bool floats)
{
	if (*frogY <= ROW_6 && *frogY > VICTORY_LANE && !floats) {
		LosingLife(screen, charset, frogX, frogY, lives);
	}
}

bool ReadKey(SDL_Keycode& key) {
	SDL_Event event;

	if (SDL_PollEvent(&event) && event.type == SDL_KEYDOWN) {
		key = event.key.keysym.sym;
		return true;
	}
	else {
		return false;
	}
}

void resetGame(int* points, int* lives, double* frogX, int* frogY, double* time, bool czy_pelne[5], GameState *state)
{
	*points = 0;
	*lives = INITIAL_LIVES;
	*frogX = INITIAL_FROG_X;
	*frogY = INITIAL_FROG_Y;
	*time = 0;
	for (int i = 0; i < 5; i++)
	{
		czy_pelne[i] = false;
	}
	state->pause = false;
	state->typed_name_for_highscores = false;
	state->quit_request = false;
}

void ControllingGame(GameState* state, double* frogX, int* frogY, int* points, double* time, int* lives, bool czy_pelne[5])
{
	SDL_Keycode key;
	while (ReadKey(key)) {
		switch (key) {
		case SDLK_ESCAPE: state->quit = 1; break;
		case SDLK_UP:
			if (state->pause == false && !state->has_lost)
			{
				*frogY -= RUCH_PIKSELE;
				*points += 10;
				break;
			}
		case SDLK_DOWN:
			if (state->pause == false && !state->has_lost)
			{
				*frogY += RUCH_PIKSELE;
				*points -= 10;
				break;
			}
		case SDLK_LEFT:
			if (state->pause == false && !state->has_lost)
			{
				*frogX -= RUCH_PIKSELE;
				break;
			}
		case SDLK_RIGHT:
			if (state->pause == false && !state->has_lost)
			{
				*frogX += RUCH_PIKSELE;
				break;
			}
		case SDLK_p:
			if (state->pause == true)
			{
				state->pause = false; break;
			}
			else
			{
				state->pause = true; break;
			}
		case SDLK_q:
			state->pause = true;
			state->quit_request = true;
			break;
		case SDLK_y:
			if (state->quit_request == true)
				state->is_in_menu = true;
			if (state->has_lost == true)
				state->quit = 1;
			break;
		case SDLK_n:
			if (state->quit_request == true)
			{
				state->pause = false;
				state->quit_request = false;
			}
			if (state->has_lost == true)
			{
				resetGame(points, lives, frogX, frogY, time, czy_pelne, state);
				
			}
			break;
		case SDLK_h:
			if (state->is_in_menu == true && state->is_in_highscores == false)
				state->is_in_highscores = true;  break;
		case SDLK_BACKSPACE:
			if (state->is_in_menu == true && state->is_in_highscores == true)
				state->is_in_highscores = false; break;

		case SDLK_KP_ENTER: case SDLK_RETURN:

			if (state->is_in_menu == true && state->is_in_highscores == false)
			{
				resetGame(points, lives, frogX, frogY, time, czy_pelne, state);
				state->is_in_menu = false;
				state->pause = false;
				*time = 0;
			}
			break;
		case SDL_QUIT:
			state->quit = 1;
			break;
		}
	}
}


typedef struct hs
{
	char nick[32];
	int points;
} TopScores;

void AddToHighScoresSorted(int score, char name[32])
{
	TopScores best[NUMBER_OF_HIGH_SCORES+1];

	FILE* pFile;
	pFile = fopen("scores.txt", "r");
	
	for (int i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
	{
		if (fscanf(pFile, "%s %d", best[i].nick, &(best[i].points)) == EOF) {
			best[i].nick[0] = '\0';
			best[i].points = 0;
		}
	}
	fclose(pFile);

	for (int i = (NUMBER_OF_HIGH_SCORES-1); i >= 0; i--)
	{
		if (score > best[i].points)
		{
			best[i + 1] = best[i];

			strcpy(best[i].nick, name);
			best[i].points = score;
		}
		else
		{
			strcpy(best[i + 1].nick, name);
			best[i + 1].points = score;
			break;
		}
	}
	pFile = fopen("scores.txt", "w");

	for (int i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
	{
		char results[100];
		sprintf_s(results, 100, "%s \t %d \n", best[i].nick, best[i].points);
		fputs(results, pFile);
	}
	fclose(pFile);
}

void SaveScore(SDL_Surface* screen, SDL_Surface* charset, SDL_Texture* scrtex, SDL_Renderer* renderer, int points)
{
	char name[32] = { '\0' };
	char buf[256];
	int index = 0;
	int end = 0;
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);


	while (!end)
	{
		SDL_Keycode key;
		while (ReadKey(key)) {
			if (key >= SDLK_a && key <= SDLK_z) {
				name[index++] = key - 'a' + 'A';
			}
			if ((key == SDLK_BACKSPACE) && index != 0) {
				index--;
			}
			name[index] = '\0';
			if (key == SDLK_RETURN && name[0] != '\0')
			{
				end = 1;
			}
		}

		SDL_FillRect(screen, NULL, czarny);
		sprintf_s(buf, 256, "YOUR NAME: %s", name);
		DrawString(screen, 160, 400, buf, charset);
		DrawFrame(scrtex, screen, renderer);
	}

	AddToHighScoresSorted(points, name);
}

void ShowHighscores(SDL_Surface *screen, SDL_Surface *charset, GameState *state)
{
	if (state->is_in_highscores == true)
	{
		int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
		SDL_FillRect(screen, NULL, czarny);
		TopScores best[10];

		FILE* pFile;
		pFile = fopen("scores.txt", "r");

		for (int i = 0; i < NUMBER_OF_HIGH_SCORES; i++)
		{
			if (pFile != NULL && fscanf(pFile, "%s %d", best[i].nick, &(best[i].points)) == EOF) {
				best[i].nick[0] = '\0';
				best[i].points = 0;
			}
		}
		if (pFile != NULL) {
			fclose(pFile);
		}

		for (int i = 1; i <= NUMBER_OF_HIGH_SCORES; i++)
		{
			char name[100], result[20];
			sprintf_s(name, 100, "%d. %s", i, best[i-1].nick);
			DrawString(screen, 50, 40*i, name, charset);
			sprintf_s(result, 20, "%d", best[i-1].points);
			DrawString(screen, 300, 40*i, result, charset);
		}
	}
}

void ShowMenu(SDL_Surface* charset, SDL_Surface* screen, GameState* state, SDL_Surface* menu)
{
	if (state->is_in_menu == true)
	{
		DrawSurface(screen, menu, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}
}


// main
#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char** argv) {
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance;

	SDL_Surface* screen, * charset, * menu;
	SDL_Surface* mapa;
	SDL_Surface* frog, * carR, * carB, * turtle, * bal;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	srand(unsigned int(time(NULL)));

	printf("wyjscie printfa trafia do tego okienka\n");
	printf("printf output goes here\n");

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}
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
	SDL_SetWindowTitle(window, "FROGGER");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	charset = SDL_LoadBMP("../template-project-2/cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);

	menu = SDL_LoadBMP("../template-project-2/menu.bmp");
	mapa = SDL_LoadBMP("../template-project-2/mapa.bmp");
	frog = SDL_LoadBMP("../template-project-2/zaba.bmp");
	carR = SDL_LoadBMP("../template-project-2/car-red.bmp");
	carB = SDL_LoadBMP("../template-project-2/car-blue.bmp");
	turtle = SDL_LoadBMP("../template-project-2/zolw.bmp");
	bal = SDL_LoadBMP("../template-project-2/bal.bmp");
	CheckTextures(charset, menu, screen, scrtex, window, renderer, mapa, frog, carR, carB, turtle, bal);

	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;

	double frogX = INITIAL_FROG_X;
	int frogY = INITIAL_FROG_Y;

	GameState state;
	state.is_in_menu = true;

	Movable movables[MOVABLES_COUNT];

	generateSet(carR, carB, turtle, bal, movables);

	bool pola_koncowe[5] = { false, false, false, false, false };
	int lives_remaining = INITIAL_LIVES;

	int scores = 0;

	while (!state.quit) {
		t2 = SDL_GetTicks();

		if (state.pause == false)
			delta = (t2 - t1) * 0.001;
		else
			delta = 0;

		t1 = t2;

		state.has_lost = GameOver(screen, charset, scores, lives_remaining);

		if (state.has_lost && !state.typed_name_for_highscores) {
			SaveScore (screen, charset, scrtex, renderer, scores);   
			state.typed_name_for_highscores = true; 
		}
		ShowMenu(charset, screen, &state, menu);
		ShowHighscores(screen, charset, &state);

		if (!state.is_in_menu && !state.has_lost && !state.typed_name_for_highscores)
		{
			SDL_FillRect(screen, NULL, czarny);

			DrawSurface(screen, mapa, SCREEN_WIDTH / 2, 16 + SCREEN_HEIGHT / 2);

			if (state.pause == true)
			{
				char* info = "PAUSE";
				DrawString(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, info, charset);
			}
			if (state.quit_request == true)
			{
				char* q = "QUIT GAME? \n Y/N";
				DrawString(screen, SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + 25, q, charset);
			}

			worldTime += delta;

			DrawTime(screen, 200, 8, &worldTime, charset, &lives_remaining, &frogX, &frogY);
			DrawLives(screen, charset, lives_remaining);
			DrawPoints(screen, charset, scores);

			bool stands_on_floating = false;

			MovablesMoveAndCollision(screen, charset, movables, &frogX, &frogY, &lives_remaining, delta, &stands_on_floating);

			IsFrogInWater(screen, charset, &frogX, &frogY, &lives_remaining, stands_on_floating);

			DrawSurface(screen, frog, frogX, frogY);

			IsAnythingEmpty(pola_koncowe);

			VictoryLaneCheck(screen, charset, pola_koncowe, &frogX, &frogY, &scores, &worldTime, &lives_remaining);

			DrawAtFinish(screen, frog, pola_koncowe);

			IsFrogOutsideTheMap(screen, charset, &frogX, &frogY, &lives_remaining);

			fpsTimer += delta;
			if (fpsTimer > 0.5) {
				fps = frames * 2;
				frames = 0;
				fpsTimer -= 0.5;
			}}

		DrawFrame(scrtex, screen, renderer);		
		ControllingGame(&state, &frogX, &frogY, &scores, &worldTime, &lives_remaining, pola_koncowe);
		frames++;
	};

	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
};
