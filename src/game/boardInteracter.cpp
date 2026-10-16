/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "boardInteracter.hpp"


BoardInteracter::BoardInteracter(const Window& _window)
: Template(_window),
cellRect{0.5f*_window.getWidth(), 0.5f*_window.getHeight(), 40.0f, 40.0f},
holdingCell{},
holdingCellRect{0.0f, 0.0f, cellRect.w, cellRect.h} {}

void BoardInteracter::reset() {
    board.reset();
    showState = ShowState::Normal;
}

void BoardInteracter::update() {
    // Updating physic
    board.update();
    //board.applyChanges();
}

void BoardInteracter::blit() const {
    // Dra odd cells
    const SDL_FRect rect = grid.absolute(texture.getRect());
    for (int x=0; x < board.getWidth(); ++x) {
        for (int y=0; y < board.getHeight()/2; ++y) {
            texture.blit(rect);
            rect.y += rect.h;
        }
    }

    // Dra even cells

    blitLines();
}
