#ifndef PJ_GAME_H
#define PJ_GAME_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

#include"screens.h"

#define PJ_FPS 60

#define PJ_WIDTH 128
#define PJ_HEIGHT 72

#define PJ_FULLSCR SDL_WINDOW_FULLSCREEN_DESKTOP
#define PJ_NOFULLSCR 0

namespace pj {

extern TTF_Font* FONT_CODE_BOLD;
extern Mix_Music* BGM;

class game {
	private:
	    int quit = 0;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Event sdlevent;

        pj::screen* screen = NULL;
	public:
        pj::screeningame* screen_ingame;

		game();
		virtual ~game();

		void start();
		void loop();

		void set_screen(pj::screen* scr);

        void restartingame();
};
}

#endif // PJ_GAME_H
