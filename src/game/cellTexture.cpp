/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "cellTexture.hpp"


CellTexture::CellTexture(const Window& _window, float _side, Color _center, Color _border)
: window(_window) {
    // Create texture
    float height = _side*SDL_sqrtf(3);
    texture = window.createTexture(height, _side);
    window.setRenderTarget(texture);

    // Inner part
    window.setDrawColor(_center);
    /*for (int i=0; i < _side/2; ++i) {
        window.drawLine(height/2-i*2, i, side/2+i*2, i);
    }*/

    SDL_FColor color = {0.0, 0.0, 0.0, 1.0};

    SDL_Vertex vertexes[] = {
        {{height/4,   0}, color},
        {{height*3/4, 0}, color},
        {{0,          _side/2}, color},
        {{height,     _side/2}, color},
        {{height/4,   _side}, color},
        {{height*3/4, _side}, color},
    };

    // External frame
    /*window.setDrawColor(BLACK);
    window.drawLine(height/4,   0,       height*3/4, 0);
    window.drawLine(height/4,   0,       0,          _side/2);
    window.drawLine(height*3/4, 0,       0,          _side/2);
    window.drawLine(0,          _side/2, height/4,   _side);
    window.drawLine(0,          _side/2, height*3/4, _side);
    window.drawLine(height/4,   _side,   height*3/4, _side);*/

    //SDL_RenderRects

    window.drawGeometry(vertexes, SDL_arraysize(vertexes));

    window.resetRenderTarget();
}

CellTexture::~CellTexture() {
    SDL_DestroyTexture(texture);
}

void CellTexture::blit(const SDL_FPoint p) const {
    SDL_FRect rect = {p.x, p.y, float(texture->w), float(texture->h)};
    window.blit(texture, rect);
}
