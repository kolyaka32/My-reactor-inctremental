/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "boardInteracter.hpp"


BoardInteracter::BoardInteracter(const Window& _window)
: Template(_window),
board(11, 10),
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
    /*board.applyForces();
    board.project();
    board.advect();*/
    //board.applyChanges();
}

void BoardInteracter::checkNext() {
    board.checkNext();
}

void BoardInteracter::blit() const {
    // ! Should get borders to less drowing
    window.setDrawColor(BLACK);
    SDL_FRect oddRectLocal = grid.absolute(oddRect);
    SDL_FRect evenRectLocal = grid.absolute(evenRect);
    switch (showState) {
    case ShowState::Thermal:
        board.blitBlockThermal(window, oddRectLocal, 0);
        board.blitBlockThermal(window, evenRectLocal, 1);
        break;

    case ShowState::Pressure:
        board.blitBlockPressure(window, oddRectLocal, 0);
        board.blitBlockPressure(window, evenRectLocal, 1);
        break;

    case ShowState::Random:
        board.blitBlockRandom(window, oddRectLocal, 0);
        board.blitBlockRandom(window, evenRectLocal, 1);
        break;

    default:
        board.blitBlockNormal(window, oddRectLocal, 0);
        board.blitBlockNormal(window, evenRectLocal, 1);
        break;
    }
    board.blitLines(window, oddRectLocal, evenRectLocal);
}
