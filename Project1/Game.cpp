#include "Game.hpp"

bool Game::running()
{
	return isRunning;
}

Game::Game()
{
	this->reset();
}

Game::~Game()
{
	SDL_FreeSurface(surfaceHead);
	SDL_FreeSurface(surfaceBody);
	SDL_FreeSurface(surfaceApple);
	delete snake;

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

void Game::init(const char* Title, int xpos, int ypos, int width, int height, bool fullscreen)
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

//	SDL_Surface* tmpSurface = IMG_Load("res/head.png");
//	textures.push_back(SDL_CreateTextureFromSurface(renderer, tmpSurface));
//	SDL_FreeSurface(tmpSurface);
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
	
	textures.push_back({ snake->Elements[0].x ,snake->Elements[0].y,SDL_CreateTextureFromSurface(renderer,surfaceHead) });
	for (int i = 1; i < snake->Elements.size();i++) {
		textures.push_back({ snake->Elements[i].x ,snake->Elements[i].y,SDL_CreateTextureFromSurface(renderer,surfaceBody) });
	}
	for (Eatable* e : eatables) {
		switch (e->getEatableType())
		{
		case EatableType::APPLE:
			textures.push_back({ e->getX(),e->getY(), SDL_CreateTextureFromSurface(renderer,surfaceApple) });
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
		SDL_DestroyTexture(texture.texture);
	//	delete texture;
	}
	textures.clear();

	renderScore(score, width - width / 10, height / 20, RenderDirection::RIGHT_TO_LEFT, score >= highscore ? NumberRenderColors::GREEN : NumberRenderColors::RED);
	renderScore(highscore, width / 10, height / 20, RenderDirection::LEFT_TO_RIGHT, NumberRenderColors::GREEN);

	if (gameOver) {
		auto texGameOver = SDL_CreateTextureFromSurface(renderer, surfaceGameOver);
		targetRect.x = width / 2 - 118;
		targetRect.y = height / 2 - 118;
		targetRect.w = 236;
		targetRect.h = 236;
		SDL_RenderCopy(renderer, texGameOver, NULL, &targetRect);
		SDL_DestroyTexture(texGameOver);
	}
		


	//SDL_DestroyTexture(texnumbers);
	// add stuff to render
	SDL_RenderPresent(renderer);

}
void Game::renderScore(int displayScore,int targetX,int targetY, RenderDirection direction,NumberRenderColors color) { //TODO: leftToRight needs implementation

	SDL_Surface* numSurface=nullptr;
	switch (color) {
		case NumberRenderColors::GREEN:
			numSurface = surfaceNumbersGreen;
			break;
		case NumberRenderColors::RED:
			numSurface = surfaceNumbersRed;
			break;
		default:
			numSurface = surfaceNumbersRed;
			break;
	}
	uint tmpscore = displayScore < 0 ? displayScore * -1 : displayScore;
	if (direction == RenderDirection::LEFT_TO_RIGHT) {
		targetX += numDigits(displayScore) * 11;
	}
	int i = 0;
	auto texNumber = SDL_CreateTextureFromSurface(renderer, numSurface);

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
		SDL_RenderCopy(renderer, texNumber, &srcRect, &targetRect);
	} while (tmpscore > 0);
	SDL_DestroyTexture(texNumber);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned";
}
