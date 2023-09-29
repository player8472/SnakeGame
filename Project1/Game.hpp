#ifndef Game_hpp
#define Game_hpp

#ifndef SDL
#include "SDL.h"
#endif // !SDL

#include "SDL_image.h"
#include <stdio.h>
#include <iostream>
#include "Snake.hpp"
#include <vector>
#include "SDL_ttf.h"
#include "Eatable.hpp"
#include "snakeglobals.h"


struct positionedtexture {
	int x;
	int y;
	SDL_Texture* texture;
};

enum RenderDirection{LEFT_TO_RIGHT,RIGHT_TO_LEFT};

class Game
{
public:
	Game();
	~Game();

	void init(const char* Title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	void reset();
	
	bool running();

private:
	const char* RES_HEAD = "res/head.png";
	const char* RES_BODY = "res/head.png";
	const char* APPLE = "res/apple.png";
	const char* GAME_OVER = "res/gameOver.png";
	const char* NUMBERS_RED = "res/numbers_red.png";
	const char* NUMBERS_GREEN = "res/numbers_green.png";

	int width;
	int height;

	uint count=0;
	bool isRunning=false;
	bool gameOver=false;
	bool isPaused = false;
	Uint32 lastUpdateTick = 0;
	SDL_Window* window=NULL;
	SDL_Renderer* renderer=NULL;
	std::vector<Eatable*> eatables;
	bool appleExists = false;
	int highscore = 0;
	int score=0;
	uint applesEaten=0;

	SDL_Surface* surfaceHead = IMG_Load(RES_HEAD);
	SDL_Surface* surfaceBody = IMG_Load(RES_BODY);
	SDL_Surface* surfaceApple = IMG_Load(APPLE);
	SDL_Surface* surfaceGameOver = IMG_Load(GAME_OVER);
	SDL_Surface* surfaceNumbersRed = IMG_Load(NUMBERS_RED);
	SDL_Surface* surfaceNumbersGreen = IMG_Load(NUMBERS_GREEN);
	std::vector<positionedtexture> textures;
	void handleKeyPress(SDL_KeyboardEvent* key);
	void renderScore(int displayScore, int targetX, int targetY, RenderDirection direction, NumberRenderColors color);
	Snake* snake;

	void createTextures();


};

#endif // !Game_hpp
