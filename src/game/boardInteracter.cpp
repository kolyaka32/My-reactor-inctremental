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
    //showState = ShowState::Normal;
}

void BoardInteracter::setShowState(unsigned _state) {
    showState = (ShowState)_state;
}

SDL_Point BoardInteracter::getPosition(const Mouse _mouse) const {
    SDL_FPoint local = grid.local(_mouse);
    local.x -= oddRect.w/2;
    local.y -= oddRect.h/2;

    float fractional_q = local.x*4/oddRect.w/3;
    float fractional_r = local.y/oddRect.h - local.x*2/oddRect.w/3;

    int q = SDL_roundf(fractional_q);
    int r = SDL_roundf(fractional_r);

    if (q % 2 == 0) {
        return {q, r+q/2};
    }
    return {q, r+(q-1)/2};
}

bool BoardInteracter::isLegimate(SDL_Point _pos) const {
    return board.isLegimate(_pos);
}

CellInfo BoardInteracter::getCellInfo(SDL_Point _pos) const {
    return board.getCellInfo(_pos);
}

void BoardInteracter::applyMovement(SDL_Point _pos, float _val) {
    board.applyMovement(_pos, _val);
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
    const SDL_FRect oddRectLocal = grid.absolute(oddRect);
    const SDL_FRect evenRectLocal = grid.absolute(evenRect);
    switch (showState) {
    case ShowState::Thermal:
        board.blitBlockThermal(window, oddRectLocal, 0);
        board.blitBlockThermal(window, evenRectLocal, 1);
        break;

    case ShowState::Pressure:
        board.blitBlockPressure(window, oddRectLocal, 0);
        board.blitBlockPressure(window, evenRectLocal, 1);
        break;

    case ShowState::Speed:
        board.blitBlockSpeed(window, oddRectLocal, 0);
        board.blitBlockSpeed(window, evenRectLocal, 1);
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
