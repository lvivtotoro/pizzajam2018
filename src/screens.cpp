#include"screens.h"

#include"game.h"

#include<SDL_image.h>
#include<string>
#include<math.h>
#include<stdio.h>

namespace pj {

screeningame::screeningame(pj::game* game, SDL_Renderer* renderer) : screen(game, renderer), phys(b2Vec2(0.0f, 0.0f)), debug(new debugdrawer(renderer)) {
    SDL_Surface* surf;
    for(int i = 0;; i++) {
        char buf[64];
        sprintf(buf, "parallax%i.png", i);

        surf = IMG_Load(buf);
        if(!surf)
            break;
        this->backgrounds.push_back(SDL_CreateTextureFromSurface(renderer, surf));

        SDL_FreeSurface(surf);
        surf = NULL;
    }

    surf = IMG_Load("sphere.png");
    this->sphereTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("player.png");
    this->playerTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("groundtop.png");
    this->groundTopTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    surf = IMG_Load("groundbottom.png");
    this->groundBottomTexture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    this->player = makeBlock(16, PJ_HEIGHT / 2, 8, 8, b2_dynamicBody);
    this->player->GetFixtureList()[0].SetDensity(0.1f);

    surf = TTF_RenderUTF8_Solid(pj::FONT_CODE_BOLD, "YOU HAVE LOSTED", {0, 0, 0});
    this->textYouLost = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);

    b2BodyDef bdef;
    bdef.type = b2_staticBody;
    bdef.position.Set(0, 0);
    bdef.angle = 0;
    this->ground = this->phys.CreateBody(&bdef);
    this->makeBlockFixture(this->ground, 0, PJ_HEIGHT / 2 - 2, PJ_WIDTH * 2, 16);
    this->makeBlockFixture(this->ground, 0, -PJ_HEIGHT / 2 + 2, PJ_WIDTH * 2, 16);

    this->debug->AppendFlags(b2Draw::e_aabbBit);
    this->phys.SetDebugDraw(this->debug);
}

screeningame::~screeningame() {
    delete this->debug;

    for(auto& bg : this->backgrounds)
        SDL_DestroyTexture(bg);
    SDL_DestroyTexture(this->textYouLost);
    SDL_DestroyTexture(this->playerTexture);
    SDL_DestroyTexture(this->sphereTexture);
    SDL_DestroyTexture(this->groundTopTexture);
    SDL_DestroyTexture(this->groundBottomTexture);
}

b2Body* screeningame::makeBlock(int x, int y, int w, int h, b2BodyType bt) {
    b2BodyDef bdef;
    bdef.type = bt;
    bdef.position.Set(x, y);
    bdef.angle = 0;

    b2Body* ret = this->phys.CreateBody(&bdef);
    this->makeBlockFixture(ret, 0, 0, w, h);

    return ret;
}

b2Fixture* screeningame::makeBlockFixture(b2Body* b, int x, int y, int w, int h) {
    b2PolygonShape shape;
    shape.SetAsBox(w / 2.f, h / 2.f, b2Vec2(x, y), 0);

    b2FixtureDef fdef;
    fdef.shape = &shape;
    fdef.density = 1;
    fdef.friction = 0.001f;

    return b->CreateFixture(&fdef);
}

void screeningame::show(SDL_Renderer*) {}

void screeningame::hide() {}

void screeningame::render(SDL_Renderer* renderer) {
    this->phys.Step(1.0f / 30.0f, 30, 20);
    applyGravity(this->player);
    this->player->ApplyForceToCenter(b2Vec2(120, 0), true);
    this->ground->SetTransform(b2Vec2(this->player->GetPosition().x, PJ_HEIGHT / 2), 0);
    if(!((int) (rand() % 20))) {
        b2Body* b = makeBlock(this->player->GetPosition().x + PJ_WIDTH + 8, rand() % PJ_HEIGHT, 8, 8, b2_dynamicBody);
        b->SetLinearVelocity(b2Vec2(-(rand() % 5000), 0));
        this->bodies.push_back(b);
    }
    if(this->player->GetLinearVelocity().x < this->lastPlayerSpeed) {
        this->gameLost = true;
    }
    this->lastPlayerSpeed = this->player->GetLinearVelocity().x;

    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;

    for(auto i = this->backgrounds.size(); i --> 0;) { // pretty cool gotowards operator
        SDL_QueryTexture(this->backgrounds[i], NULL, NULL, &rect.w, &rect.h);
        rect.x = fmod(-player->GetPosition().x, rect.w);
        SDL_RenderCopy(renderer, this->backgrounds[i], NULL, &rect);
        rect.x += rect.w;
        SDL_RenderCopy(renderer, this->backgrounds[i], NULL, &rect);
    }

    for(auto& body : this->bodies) {
        applyGravity(body);

        rect.w = 8;
        rect.h = 8;
        rect.x = body->GetPosition().x - rect.w / 2;
        rect.y = body->GetPosition().y - rect.h / 2;
        applyCamera(&rect.x);
        SDL_RenderCopyEx(renderer, this->sphereTexture, NULL, &rect, body->GetAngle() * (180.0f / 3.1415927f), NULL, SDL_FLIP_NONE);
    }

    SDL_Rect srcrect;
    SDL_QueryTexture(this->playerTexture, NULL, NULL, &srcrect.w, &srcrect.h);
    this->playerFrame += 0.2f;
    int playerFrames = srcrect.w / srcrect.h;
    srcrect.w = srcrect.h;
    srcrect.y = 0;
    srcrect.x = srcrect.h * (int) fmod(this->playerFrame, playerFrames);

    rect.x = this->player->GetPosition().x - srcrect.w / 4;
    rect.y = this->player->GetPosition().y - srcrect.h / 4;
    applyCamera(&rect.x);
    SDL_RenderCopyEx(renderer, this->playerTexture, &srcrect, &rect, this->player->GetAngle() * (180.0f / 3.1415927f), NULL, SDL_FLIP_NONE);

    SDL_QueryTexture(this->groundTopTexture, NULL, NULL, &rect.w, &rect.h);
    rect.x = fmod(-player->GetPosition().x, rect.w);
    rect.y = 0;
    rect.y -= player->GetPosition().y - rect.h * 2 + 7;
    SDL_RenderCopy(renderer, this->groundTopTexture, NULL, &rect);
    rect.x += rect.w;
    SDL_RenderCopy(renderer, this->groundTopTexture, NULL, &rect);
    rect.x = fmod(-player->GetPosition().x, rect.w);
    rect.y = PJ_HEIGHT - rect.h;
    rect.y += (PJ_HEIGHT - rect.h) - player->GetPosition().y - 3;
    SDL_RenderCopy(renderer, this->groundBottomTexture, NULL, &rect);
    rect.x += rect.w;
    SDL_RenderCopy(renderer, this->groundBottomTexture, NULL, &rect);

    if(this->gameLost) {
        SDL_QueryTexture(this->textYouLost, NULL, NULL, &rect.w, &rect.h);
        rect.x = PJ_WIDTH / 2 - rect.w / 2;
        rect.y = PJ_HEIGHT / 2 - rect.h / 2;
        SDL_RenderCopy(renderer, this->textYouLost, NULL, &rect);
    }

    //this->phys.DrawDebugData();
}

void screeningame::event(SDL_Event& ev) {
    switch(ev.type) {
        case SDL_MOUSEBUTTONDOWN:
            if(!this->gameLost)
                this->player->ApplyLinearImpulseToCenter(b2Vec2(0, this->player->GetPosition().y >= PJ_HEIGHT / 2 ? -2000 : 2000), true);
            else
                this->game->restartingame();
            break;
    }
}

void screeningame::applyGravity(b2Body* body) {
    body->ApplyForceToCenter(b2Vec2(0, (body->GetPosition().y >= PJ_HEIGHT / 2 ? 1 : -1) * 1800.0f * body->GetGravityScale()), true);
}

void screeningame::applyCamera(int* x) {
    *x -= this->player->GetPosition().x - 4;
}

//

screenmenu::screenmenu(pj::game* game, SDL_Renderer* renderer) : screen(game, renderer) {
    SDL_Surface* surf;

    surf = TTF_RenderUTF8_Solid(pj::FONT_CODE_BOLD, "CLICK TO STARTIFY", {0, 0, 0});
    this->textStart = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
}

screenmenu::~screenmenu() {
    SDL_DestroyTexture(this->textStart);
}

void screenmenu::show(SDL_Renderer*) {}

void screenmenu::hide() {}

void screenmenu::render(SDL_Renderer* renderer) {
    SDL_Rect rect;

    SDL_QueryTexture(this->textStart, NULL, NULL, &rect.w, &rect.h);
    rect.x = PJ_WIDTH / 2 - rect.w / 2;
    rect.y = PJ_HEIGHT / 2 - rect.h / 2;

    SDL_RenderCopy(renderer, this->textStart, NULL, &rect);
}

void screenmenu::event(SDL_Event&) {
}

}
