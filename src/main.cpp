#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include"game.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING & ~SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG);

    pj::game game;
    game.start();

    game.loop();

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
	return 0;
}
