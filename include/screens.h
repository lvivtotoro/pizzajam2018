#ifndef PJ_SCREENLOGIN_H
#define PJ_SCREENLOGIN_H

#include<SDL.h>
#include<vector>

#include"Box2D/Box2D.h"

#include"debugdrawer.h"

namespace pj {

class game; // forward declaration because i fucked up

class screen {
    protected:
        pj::game* game;
    public:
        screen(pj::game* game, SDL_Renderer*) : game(game) {}
        virtual void show(SDL_Renderer*) {}
        virtual void render(SDL_Renderer*) = 0;
        virtual void event(SDL_Event&) {}
        virtual void hide() {}
};

class screeningame : public screen {
    private:
        SDL_Texture* textYouLost;

        std::vector<SDL_Texture*> backgrounds;
        SDL_Texture* sphereTexture;
        SDL_Texture* groundTopTexture;
        SDL_Texture* groundBottomTexture;

        SDL_Texture* playerTexture;
        double playerFrame = 0;

        bool gameLost = false;

        std::vector<b2Body*> bodies;

        b2World phys;
        debugdrawer* debug;

        float lastPlayerSpeed = 0; // used for losing
        b2Body* player;
        b2Body* ground;

        b2Body* makeBlock(int, int, int, int, b2BodyType);
        b2Fixture* makeBlockFixture(b2Body*, int, int, int, int);

        void applyGravity(b2Body*);
        void applyCamera(int*);
    public:
        screeningame(pj::game*, SDL_Renderer*);
        virtual ~screeningame();

        void show(SDL_Renderer*);
		void render(SDL_Renderer*);
        void event(SDL_Event&);
        void hide();
};

class screenmenu : public screen {
    private:
        SDL_Texture* textStart;
    public:
        screenmenu(pj::game*, SDL_Renderer*);
        virtual ~screenmenu();

        void show(SDL_Renderer*);
		void render(SDL_Renderer*);
        void event(SDL_Event&);
        void hide();
};

}

#endif // PJ_SCREENLOGIN_H
