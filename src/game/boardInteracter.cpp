/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "boardInteracter.hpp"


BoardInteracter::BoardInteracter(const Window& _window)
: Template(_window),
cellRect{0.5f*_window.getWidth(), 0.5f*_window.getHeight(), 40.0f, 40.0f},
texture(_window, 60.0, GREY, BLACK),
holdingCell{},
holdingCellRect{0.0f, 0.0f, cellRect.w, cellRect.h} {}

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

void BoardInteracter::blit() const {
    // Draw odd cells
    SDL_FRect rect1 = grid.absolute(texture.getRect());
    float startY1 = rect1.y;
    for (int x=0; x < board.getWidth(); ++x) {
        rect1.y = startY1;
        for (int y=0; y < board.getHeight()/2; ++y) {
            //board[2*x]
            texture.blit(rect1);
            rect1.y += rect1.h;
        }
        rect1.x += rect1.w * 1.5;
    }

    // Draw even cells
    SDL_FRect rect2 = grid.absolute(texture.getRect());
    float startY2 = rect2.y + rect2.h/2;
    rect2.x += rect2.w*3/4;
    for (int x=0; x < board.getWidth(); ++x) {
        rect2.y = startY2;
        for (int y=0; y < board.getHeight()/2; ++y) {
            texture.blit(rect2);
            rect2.y += rect2.h;
        }
        rect2.x += rect2.w * 1.5;
    }

    blitLines();
}

void BoardInteracter::blitLines() const {
    // !
}
