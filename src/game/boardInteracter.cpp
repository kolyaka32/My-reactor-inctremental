/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "boardInteracter.hpp"


BoardInteracter::BoardInteracter(const Window& _window)
: Template(_window),
board(4, 4),
oddRect{0.0, 0.0, 200.0f/SDL_sqrtf(3), 100.0},
evenRect{oddRect.w*3/4, oddRect.h/2, oddRect.w, oddRect.h},
grid(),
holdingCell{} {}

void BoardInteracter::reset() {
    board.reset();
    showState = ShowState::Normal;
}

void BoardInteracter::setShowState(unsigned _state) {
    showState = (ShowState)_state;
}

void BoardInteracter::update() {
    // Updating physic
    board.update();
    //board.applyChanges();
}

void BoardInteracter::blitUnit(SDL_FRect _rect, SDL_FColor _color) const {
    SDL_Vertex vertexes[] = {
        {{_rect.x+_rect.w/4,   _rect.y},           _color},
        {{_rect.x+_rect.w*3/4, _rect.y},           _color},
        {{_rect.x,             _rect.y+_rect.h/2}, _color},
        {{_rect.x+_rect.w,     _rect.y+_rect.h/2}, _color},
        {{_rect.x+_rect.w/4,   _rect.y+_rect.h},   _color},
        {{_rect.x+_rect.w*3/4, _rect.y+_rect.h},   _color},
    };
    int order[] = {0, 1, 2,    1, 2, 3,    2, 3, 4,    3, 4, 5};

    window.drawGeometry(vertexes, SDL_arraysize(vertexes), order, SDL_arraysize(order));
}

void BoardInteracter::blitBlock(SDL_FRect _rect, int _offset) const {
    SDL_FRect rect = grid.absolute(_rect);
    float startY = rect.y;
    for (int x=_offset; x < board.getWidth(); x+=2) {
        rect.y = startY;
        for (int y=0; y < board.getHeight(); ++y) {
            switch (showState) {
            case ShowState::Thermal:
                blitUnit(rect, board[{x, y}].getThermalColor());
                break;

            case ShowState::Pressure:
                blitUnit(rect, board[{x, y}].getPressureColor());
                break;

            case ShowState::Random:
                blitUnit(rect, board[{x, y}].getRandomColor());
                break;

            default:
                blitUnit(rect, board[{x, y}].getNormalColor());
                break;
            }
            rect.y += rect.h;
        }
        rect.x += rect.w * 1.5;
    }
}

void BoardInteracter::blitLines() const {
    // !
}

void BoardInteracter::blit() const {
    // ! Should get borders to less drowing
    blitBlock(oddRect, 0);
    blitBlock(evenRect, 1);
    blitLines();
}
