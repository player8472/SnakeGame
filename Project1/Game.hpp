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
	Game(const char* Title, int xpos, int ypos, int width, int height, bool fullscreen);
	~Game();

	void handleEvents();
	void update();
	void render();
	void clean();
	void reset();
	
	bool running();

private:
	int width;
	int height;

	SDL_Texture* texHead;
	SDL_Texture* texBody;
	SDL_Texture* texApple;
	SDL_Texture* texGameOver;
	SDL_Texture* texNumbersRed;
	SDL_Texture* texNumbersGreen;


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

	std::vector<positionedtexture> textures;
	void handleKeyPress(SDL_KeyboardEvent* key);
	void renderScore(int displayScore, int targetX, int targetY, RenderDirection direction, NumberRenderColors color);
	Snake* snake;

	void createTextures();


};

#endif // !Game_hpp
