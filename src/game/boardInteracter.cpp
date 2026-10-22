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

ChannelPos BoardInteracter::getPosition(const Mouse _mouse) const {
    SDL_FPoint local = grid.local(_mouse);
    local.x -= oddRect.w/2;
    local.y -= oddRect.h/2;

    int x = SDL_roundf(local.x*4/oddRect.w/3);
    int y = SDL_roundf(local.y/oddRect.h);

    float frac_q = -local.x*2/oddRect.w/3 + local.y/oddRect.h;
    float frac_r = local.x/oddRect.w + local.y/oddRect.h/2;
    //float frac_s = -frac_q-frac_r;

    int q = SDL_roundf(frac_q);
    int r = SDL_roundf(frac_r);
    //int s = SDL_roundf(frac_s);

    if (y % 2 == 0) {
        //
        if (SDL_fabs(r - frac_r)) {
            //
        }
    }

    /*float q_diff = SDL_fabs(q - frac_q);
    float r_diff = SDL_fabs(r - frac_r);
    float s_diff = SDL_fabs(s - frac_s);

    if (q_diff > r_diff and q_diff > s_diff) {
        q = -r - s;
    } else if (r_diff > s_diff) {
        r = -q - s;
    }*/

    // Deciding type
    int type = 0;
    const float border = 0.4;
    /*if (r % 2 == 0) {
        if (SDL_fabs(q - frac_q) > border) {
            type = 1;  // Vertical
        } else if (false) {
            type = 2;  // Left upper
        } else if (false) {
            type = 3;  // Left down
        }
    } else {
        if (SDL_fabs(q - frac_q) < 0.5-border) {
            type = 1;  // Vertical
        } else if (false) {
            type = 2;  // Left upper
        } else if (false) {
            type = 3;  // Left down
        }
        //return {type, x, SDL_fabs(q - frac_q)};
    }*/
    return {type, x, r};
}

bool BoardInteracter::isLegimate(ChannelPos _pos) const {
    return board.isLegimate(_pos);
}

CellInfo BoardInteracter::getInfo(ChannelPos _pos) const {
    return board.getInfo(_pos);
}

void BoardInteracter::applyMovement(ChannelPos _pos, float _val) {
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
