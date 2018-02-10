#ifndef DEBUGDRAWER_H
#define DEBUGDRAWER_H

#include"Box2D/Box2D.h"
#include<SDL.h>

class debugdrawer : public b2Draw {
    private:
        SDL_Renderer* renderer;
    public:
        debugdrawer(SDL_Renderer*);

        void DrawPolygon(const b2Vec2*, int32 vertexCount, const b2Color&);
        void DrawSolidPolygon(const b2Vec2*, int32 vertexCount, const b2Color&);
        void DrawCircle(const b2Vec2&, float32, const b2Color&);
        void DrawSolidCircle(const b2Vec2&, float32, const b2Vec2&, const b2Color&);
        void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&);
        void DrawTransform(const b2Transform&);
        void DrawTransform(const b2Vec2&, float32, const b2Color&);
        void DrawPoint(const b2Vec2&, float32, const b2Color&);

};

#endif // DEBUGDRAWER_H
