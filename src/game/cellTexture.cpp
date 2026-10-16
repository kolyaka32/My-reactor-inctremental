/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "cellTexture.hpp"


CellTexture::CellTexture(const Window& _window, float _side, Color _center, Color _border)
: window(_window),
side(_side),
height(_side*2/SDL_sqrtf(3)) {
    // Create texture
    texture = window.createTexture(height, side);
    window.setRenderTarget(texture);

    // Inner part
    window.setDrawColor(_center);
    /*for (int i=0; i < _side/2; ++i) {
        window.drawLine(height/2-i*2, i, side/2+i*2, i);
    }*/

    SDL_FColor color1 = {0.0, 0.0, 0.0, 1.0};
    SDL_FColor color2 = {1.0, 0.0, 0.0, 1.0};
    SDL_FColor color3 = {0.0, 1.0, 0.0, 1.0};
    SDL_FColor color4 = {0.0, 0.0, 1.0, 1.0};

    SDL_Vertex vertexes[] = {
        {{height/4,   0.0},    color1},
        {{height*3/4, 0.0},    color2},
        {{0.0,        side/2}, color2},
        {{height,     side/2}, color3},
        {{height/4,   side},   color3},
        {{height*3/4, side},   color4},
    };

    /*window.setDrawColor(_border);
    window.drawLine(vertexes[0].position, vertexes[1].position);
    window.drawLine(vertexes[0].position, vertexes[2].position);
    window.drawLine(vertexes[1].position, vertexes[3].position);
    window.drawLine(vertexes[2].position, vertexes[4].position);
    window.drawLine(vertexes[3].position, vertexes[5].position);
    window.drawLine(vertexes[4].position, vertexes[5].position);*/

    // External frame
    /*window.setDrawColor(BLACK);
    window.drawLine(height/4,   0,       height*3/4, 0);
    window.drawLine(height/4,   0,       0,          _side/2);
    window.drawLine(height*3/4, 0,       0,          _side/2);
    window.drawLine(0,          _side/2, height/4,   _side);
    window.drawLine(0,          _side/2, height*3/4, _side);
    window.drawLine(height/4,   _side,   height*3/4, _side);*/

    int order[] = {0, 1, 2,    1, 2, 3,    2, 3, 4,    3, 4, 5};

    window.drawGeometry(vertexes, SDL_arraysize(vertexes), order, SDL_arraysize(order));

    window.resetRenderTarget();
}

CellTexture::~CellTexture() {
    SDL_DestroyTexture(texture);
}

SDL_FRect CellTexture::getRect() const {
    return SDL_FRect{0.0, 0.0, height, side};
}

void CellTexture::blit(SDL_FRect _rect) const {
    window.blit(texture, _rect);
}
