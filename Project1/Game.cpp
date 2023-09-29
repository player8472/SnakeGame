#include "Game.hpp"

bool Game::running()
{
	return isRunning;
}
Game::Game(const char* Title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	this->width = width;
	this->height = height;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL Init failed";
		exit(1);
	}
	std::cout << "Subsystems Initialized";

	window = SDL_CreateWindow(Title, xpos, ypos, width, height, flags);

	if (!window) {
		std::cerr << "SDL_Window Init failed";
		exit(2);
	}
	std::cout << "Window created";

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "SDL Renderer Init failed";
		exit(3);
	}
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	std::cout << "Renderer created";
	isRunning = true;


	texApple = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_APPLE));
	texBody = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_BODY));
	texHead = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_HEAD));
	texGameOver = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_GAME_OVER));
	texNumbersGreen = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_NUMBERS_GREEN));
	texNumbersRed = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_NUMBERS_RED));
	texTxt = SDL_CreateTextureFromSurface(renderer, IMG_Load(RES_TXT));
	/* txt 
	CharWidth: 15
	CharHeight: 20+++

	*/




	this->reset();
}

Game::~Game()
{
	delete snake;
	SDL_DestroyTexture(texApple);
	SDL_DestroyTexture(texBody);
	SDL_DestroyTexture(texHead);
	SDL_DestroyTexture(texGameOver);
	SDL_DestroyTexture(texNumbersGreen);
	SDL_DestroyTexture(texNumbersRed);
	SDL_DestroyTexture(texTxt);

}

void Game::reset() {
	this->gameOver = false;
	delete snake;
	this->snake= new Snake(0, 0, Down, 3);
	for (auto e : eatables) {
		delete e;
	}
	eatables.clear();
	applesEaten = 0;
	appleExists = false;
	score = 0;
	lastUpdateTick = SDL_GetTicks();
}



void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		handleKeyPress(&event.key);

	default:
		break;
	}
}
void Game::handleKeyPress(SDL_KeyboardEvent* key)
{
	switch (key->keysym.sym) {
	case SDLK_ESCAPE:
		//if (isPaused) {
		//	lastUpdateTick = SDL_GetTicks(); 
		//}
		isPaused = !isPaused; // will be replaced with menu-functionality eventually
		break;
	case SDLK_LEFT:
		if(!gameOver)
		snake->changeDirection(CounterClockWise);
		break;
	case SDLK_RIGHT:
		if (!gameOver)
		snake->changeDirection(Clockwise);
		break;
	case SDLK_n:
		if (this->gameOver) {
			Game::reset();
		}
		break;
	}
}

void Game::update()
{
	if (!appleExists) {
		Apple* newApple=new Apple(width/32,height/32,100);
		while (snake->collisionCheck(newApple->getX(), newApple->getY(), false)) {
			newApple = new Apple(width/32, height/32, 100);			
//			std::cout << std::endl << "Apple at: (" << e->getX() << "/" << e->getY() << ")";
		}
		eatables.push_back(newApple);
		std::cout << std::endl << "Apple at: (" << newApple->getX() << "/" << newApple->getY() << ")";
		appleExists = true;

	}
	if (snake->hasHitWall(width / 32, height / 32)||snake->selfCollisioncheck()) {
		gameOver = true;
	}
	else {
		if (SDL_TICKS_PASSED(SDL_GetTicks(), lastUpdateTick + 100) && !isPaused) {
			snake->move();
			for (int i = 0; i < eatables.size();i++) {
				if (snake->collisionCheck(eatables[i]->getX(), eatables[i]->getY(), true)) {
					if (eatables[i]->getEatableType() == EatableType::APPLE) {
						applesEaten++;
					}
					snake->eat(eatables[i]->getNutrition());
					this->score += eatables[i]->getScore();
					gameOver = snake->Elements.size()+snake->getNutrition() <= 0; // if current targetsize is lower than 1, its game over
					highscore = score > highscore ? score : highscore;
					delete eatables[i]; // delete the object
					eatables.erase(eatables.begin()+i); // remove the pointer from vector		
					appleExists = false;
				}
			}
			lastUpdateTick = SDL_GetTicks();
		//	std::cout << std::endl << lastUpdateTick;
		}
	}
}



void Game::createTextures()
{
	
	textures.push_back({ snake->Elements[0].x ,snake->Elements[0].y,texHead });
	for (int i = 1; i < snake->Elements.size();i++) {
		textures.push_back({ snake->Elements[i].x ,snake->Elements[i].y,texBody });
	}
	for (Eatable* e : eatables) {
		switch (e->getEatableType())
		{
		case EatableType::APPLE:
			textures.push_back({ e->getX(),e->getY(),texApple });
			break;
		}
	}
}

void Game::render()
{
	double angle=0;
	createTextures();
	SDL_RenderClear(renderer);
	SDL_Rect targetRect{};
	for (positionedtexture texture : textures) {		
		targetRect.x = 32 * texture.x;
		targetRect.y = 32 * texture.y;
		targetRect.w = 32;
		targetRect.h = 32;

		switch (snake->getDirection()) {
		case Up:
			angle = 0;
			break;
		case Right:
			angle = 90;
			break;
		case Down:
			angle = 180;
			break;
		case Left:
			angle = 270;
			break;
		}
		//SDL_RenderCopy(renderer, texture.texture, NULL, &targetRect);
		SDL_RenderCopyEx(renderer, texture.texture, NULL, &targetRect, angle, NULL, SDL_FLIP_NONE);
	//	delete texture;
	}
	textures.clear();

	renderScore(score, width - width / 10, height / 20, RenderDirection::RIGHT_TO_LEFT, score >= highscore ? NumberRenderColors::GREEN : NumberRenderColors::RED);
	renderScore(highscore, width / 10, height / 20, RenderDirection::LEFT_TO_RIGHT, NumberRenderColors::GREEN);

	if (gameOver) {
		targetRect.x = width / 2 - 118;
		targetRect.y = height / 2 - 118;
		targetRect.w = 236;
		targetRect.h = 236;
		SDL_RenderCopy(renderer, texGameOver, NULL, &targetRect);
		//SDL_Rect tmprect{ 20,20,20,20 };
		//targetRect.w = 40;
		//targetRect.h = 40;
		//SDL_RenderCopy(renderer, texTxt,&tmprect , &targetRect);
	}
		


	//SDL_DestroyTexture(texnumbers);
	// add stuff to render
	SDL_RenderPresent(renderer);

}
void Game::renderScore(int displayScore,int targetX,int targetY, RenderDirection direction,NumberRenderColors color) { //TODO: leftToRight needs implementation

	SDL_Texture* texNum=nullptr;
	switch (color) {
		case NumberRenderColors::GREEN:
			texNum = texNumbersGreen;
			break;
		case NumberRenderColors::RED:
			texNum = texNumbersRed;
			break;
		default:
			texNum = texNumbersRed;
			break;
	}
	uint tmpscore = displayScore < 0 ? displayScore * -1 : displayScore;
	if (direction == RenderDirection::LEFT_TO_RIGHT) {
		targetX += numDigits(displayScore) * 11;
	}
	int i = 0;

	do {
		SDL_Rect targetRect{};
		targetRect.x = targetX - i * 11;
		targetRect.y = targetY;
		targetRect.w = 9;
		targetRect.h = 15;

		SDL_Rect srcRect{};
		uint displaynumber;
		if (tmpscore == 0) {
			displaynumber = 0;
		}
		else {
			displaynumber = tmpscore % 10;
			tmpscore /= 10;
			++i;
		}
		srcRect.x = displaynumber * 3;
		srcRect.y = 0;
		srcRect.w = 3;
		srcRect.h = 5;
		SDL_RenderCopy(renderer, texNum, &srcRect, &targetRect);
	} while (tmpscore > 0);

}

void Game::renderString(std::string str, int posx, int posy, DefinedPosition pos,int sizeFactor)
{
	int startposx = posx;
	int startposy = posy;
	switch (pos) {
	case RIGHT:
		startposx -= str.length() * 20*sizeFactor;
		break;
	case CENTRAL:
		startposx -= str.length() * 10 * sizeFactor;
		break;
	}

}

SDL_Rect* Game::LetterToTextRect(char c)
{
	SDL_Rect RetRect{};
	RetRect.w = 20;
	RetRect.h = 20;
	switch (c) {
	case 'a':
		RetRect.x = 3*20;
		RetRect.y = 2*20;
		break;
	case 'e':
		RetRect.x = 7 * 20;
		RetRect.y = 2 * 20;
		break;
	case 'f':
		RetRect.x = 8 * 20;
		RetRect.y = 2 * 20;
		break;
	case 'g':
		RetRect.x = 9 * 20;
		RetRect.y = 2 * 20;
		break;
	case 'm':
		RetRect.x = 0 * 20;
		RetRect.y = 3 * 20;
		break;
	case 'n':
		RetRect.x = 1 * 20;
		RetRect.y = 3 * 20;
		break;
	case 'o':
		RetRect.x = 2 * 20;
		RetRect.y = 3 * 20;
		break;
	case 'p':
		RetRect.x = 3 * 20;
		RetRect.y = 3 * 20;
		break;
	case 'r':
		RetRect.x = 5 * 20;
		RetRect.y = 3 * 20;
		break;
	case 's':
		RetRect.x = 2 * 20;
		RetRect.y = 6 * 20;
		break;
	case 'w':
		RetRect.x = 2 * 20;
		RetRect.y = 9 * 20;
		break;
	case ' ':
	default:
		RetRect.x = 0;
		RetRect.y = 0;
		break;

	}
	return &RetRect;
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned";
}

