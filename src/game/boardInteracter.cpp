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
    board.applyForces();
    board.project();
    board.advect();
    //board.applyChanges();
}

void BoardInteracter::blit() const {
    // ! Should get borders to less drowing
    window.setDrawColor(BLACK);
    switch (showState) {
    case ShowState::Thermal:
        board.blitBlockThermal(window, grid.absolute(oddRect), 0);
        board.blitBlockThermal(window, grid.absolute(evenRect), 1);
        break;

    case ShowState::Pressure:
        board.blitBlockPressure(window, grid.absolute(oddRect), 0);
        board.blitBlockPressure(window, grid.absolute(evenRect), 1);
        break;

    case ShowState::Random:
        board.blitBlockRandom(window, grid.absolute(oddRect), 0);
        board.blitBlockRandom(window, grid.absolute(evenRect), 1);
        break;

    default:
        board.blitBlockNormal(window, grid.absolute(oddRect), 0);
        board.blitBlockNormal(window, grid.absolute(evenRect), 1);
        break;
    }
}
