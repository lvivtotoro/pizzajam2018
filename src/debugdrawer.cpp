#include "debugdrawer.h"
#include<stdio.h>

debugdrawer::debugdrawer(SDL_Renderer* renderer) : b2Draw(), renderer(renderer) {}

void debugdrawer::DrawPolygon(const b2Vec2* verts, int32 vertexCount, const b2Color& c) {
    for(int i = 1; i < vertexCount; i++) {
        this->DrawSegment(verts[i - 1], verts[i], c);
    }
        this->DrawSegment(verts[0], verts[vertexCount - 1], c);
}
void debugdrawer::DrawSolidPolygon(const b2Vec2* verts, int32 vertexCount, const b2Color& c) {
    for(int i = 1; i < vertexCount; i++) {
        this->DrawSegment(verts[i - 1], verts[i], c);
    }
}
void debugdrawer::DrawCircle(const b2Vec2&, float32, const b2Color&) {
}
void debugdrawer::DrawSolidCircle(const b2Vec2&, float32, const b2Vec2&, const b2Color&) {
}
void debugdrawer::DrawSegment(const b2Vec2& x, const b2Vec2& y, const b2Color& c) {
    SDL_SetRenderDrawColor(this->renderer, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(this->renderer, x.x, x.y, y.x, y.y);
}
void debugdrawer::DrawTransform(const b2Transform&) {
}
void debugdrawer::DrawTransform(const b2Vec2&, float32, const b2Color&) {
}
void debugdrawer::DrawPoint(const b2Vec2&, float32, const b2Color&) {
}
