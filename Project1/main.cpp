#include "Game.hpp"

Game* game = nullptr;


int main(int argc, char* argv[]) {
	game = new Game();
	game->init("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1056, 1056, false);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}
	game->clean();

	return 0;

}