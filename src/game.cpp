#include"game.h"

#include"screens.h"
#include<stdio.h>
#include<time.h>

namespace pj {

TTF_Font* FONT_CODE_BOLD = NULL;
Mix_Music* BGM = NULL;

game::game() {
	this->window = SDL_CreateWindow("Gravity Flip 2 (Pizza Jam 2018 Entry)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, PJ_WIDTH, PJ_HEIGHT, PJ_NOFULLSCR | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_PRESENTVSYNC);

	SDL_RenderSetLogicalSize(this->renderer, PJ_WIDTH, PJ_HEIGHT);

    pj::FONT_CODE_BOLD = TTF_OpenFont("CODE Bold.otf", 14);
    pj::BGM = Mix_LoadMUS("etherealpsyche.living.ogg");

    srand(time(NULL));

    this->screen_ingame = new pj::screeningame(this, this->renderer);
}

game::~game() {
	delete this->screen_ingame;

	SDL_DestroyRenderer(this->renderer);
	SDL_DestroyWindow(this->window);
}

void game::start() {
	SDL_ShowWindow(this->window);

	Mix_PlayMusic(pj::BGM, -1);

	this->set_screen(this->screen_ingame);
}

void game::loop() {
	while(!this->quit) {
        SDL_RenderClear(this->renderer);
		if(this->screen != NULL) {
			this->screen->render(this->renderer);
		}
        SDL_RenderPresent(this->renderer);

		while(SDL_PollEvent(&this->sdlevent)) {
			switch(this->sdlevent.type) {
				case SDL_QUIT:
                    this->quit = 1;
                    break;
			}
            if(this->screen != NULL) {
                this->screen->event(this->sdlevent);
            }
		}

		SDL_Delay(1000 / PJ_FPS);
	}
}

void game::set_screen(pj::screen* scr) {
    if(this->screen != NULL)
        this->screen->hide();

	this->screen = scr;

	if(this->screen != NULL)
        this->screen->show(this->renderer);
}

void game::restartingame() {
    delete this->screen_ingame;
    this->screen_ingame = new pj::screeningame(this, this->renderer);
}

}
