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

SDL_FPoint BoardInteracter::getPosition(const Mouse _mouse) const {
    SDL_FPoint local = grid.local(_mouse);
    local.x -= oddRect.w/2;
    local.y -= oddRect.h/2;

    float fractional_q = local.y/oddRect.h - local.x*2/oddRect.w/3;
    float fractional_r = -local.y/oddRect.h + local.x*2/oddRect.w/3;
    float fractional_s = -fractional_q - fractional_r;

    int q = SDL_roundf(fractional_q);
    int r = SDL_roundf(fractional_r);
    int s = SDL_roundf(fractional_s);

    return {fractional_q, fractional_r};

    float q_diff = abs(q - fractional_q);
    float r_diff = abs(r - fractional_r);
    float s_diff = abs(s - fractional_s);

    if (q_diff > r_diff && q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    }
    /*if (q % 2 == 0) {
        return {q, q+r/2};
    }
    return {q, q+(r+1)/2};*/
}

bool BoardInteracter::isLegimate(SDL_Point _pos) const {
    return board.isLegimate(_pos);
}

float BoardInteracter::getTemperature(SDL_Point _pos) const {
    return board.getTemperature(_pos);
}

float BoardInteracter::getPressure(SDL_Point _pos) const {
    return board.getPressure(_pos);
}

float BoardInteracter::getSpeed(SDL_Point _pos) const {
    return board.getSpeed(_pos);
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
